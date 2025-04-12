#include "camera_handler.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <cstring>
#include <vector>
#include <string>

CameraHandler::CameraHandler(const char *camera_url) : format_ctx(nullptr), codec_ctx(nullptr), sws_ctx(nullptr), video_stream_index(-1)
{
    av_register_all();
    avformat_network_init();

    if (avformat_open_input(&format_ctx, camera_url, nullptr, nullptr) != 0)
    {
        throw std::runtime_error("Failed to open camera");
    }

    if (avformat_find_stream_info(format_ctx, nullptr) < 0)
    {
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Failed to retrieve stream info");
    }

    for (unsigned int i = 0; i < format_ctx->nb_streams; i++)
    {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1)
    {
        avformat_close_input(&format_ctx);
        throw std::runtime_error("No video stream found");
    }

    AVCodecParameters *codec_params = format_ctx->streams[video_stream_index]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec)
    {
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Unsupported codec");
    }

    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Failed to allocate codec context");
    }

    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0)
    {
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Failed to copy codec parameters");
    }

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
    {
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Failed to open codec");
    }

    frame = av_frame_alloc();
    if (!frame)
    {
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        throw std::runtime_error("Failed to allocate frame");
    }
}

CameraHandler::~CameraHandler()
{
    av_frame_free(&frame);
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
}

bool CameraHandler::getNextFrame(uint8_t **rgb_data, int &width, int &height, int &channels)
{
    AVPacket packet;
    while (av_read_frame(format_ctx, &packet) >= 0)
    {
        if (packet.stream_index == video_stream_index)
        {
            if (avcodec_send_packet(codec_ctx, &packet) == 0)
            {
                while (avcodec_receive_frame(codec_ctx, frame) == 0)
                {
                    if (!sws_ctx)
                    {
                        sws_ctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format,
                                                 frame->width, frame->height, AV_PIX_FMT_RGB24,
                                                 SWS_BILINEAR, nullptr, nullptr, nullptr);
                    }

                    width = frame->width;
                    height = frame->height;
                    channels = 3;

                    int rgb_linesize[4];
                    av_image_alloc(rgb_data, rgb_linesize, width, height, AV_PIX_FMT_RGB24, 1);

                    sws_scale(sws_ctx, frame->data, frame->linesize, 0, height, rgb_data, rgb_linesize);

                    av_packet_unref(&packet);
                    return true;
                }
            }
        }
        av_packet_unref(&packet);
    }
    return false;
}

std::vector<std::string> CameraHandler::listAvailableCameras()
{
    std::vector<std::string> availableCameras;

    for (int i = 0; i < 64; ++i) // Iterate over possible video devices
    {
        std::string devicePath = "/dev/video" + std::to_string(i);
        int fd = open(devicePath.c_str(), O_RDONLY);
        if (fd == -1)
        {
            continue; // Skip if the device cannot be opened
        }

        struct v4l2_capability cap;
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == 0)
        {
            if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
            {
                availableCameras.push_back(devicePath);
            }
        }

        close(fd);
    }

    return availableCameras;
}