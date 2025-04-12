// Copyright (c) 2021 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "inference_handler.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <rknn model>\n", argv[0]);
        return -1;
    }

    const char *model_path = argv[1];
    const char *camera_url = "video=0"; // Adjust this for your camera input.

    av_register_all();
    avformat_network_init();

    AVFormatContext *format_ctx = nullptr;
    if (avformat_open_input(&format_ctx, camera_url, nullptr, nullptr) != 0)
    {
        printf("Failed to open camera\n");
        return -1;
    }

    if (avformat_find_stream_info(format_ctx, nullptr) < 0)
    {
        printf("Failed to retrieve stream info\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    int video_stream_index = -1;
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
        printf("No video stream found\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVCodecParameters *codec_params = format_ctx->streams[video_stream_index]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec)
    {
        printf("Unsupported codec\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        printf("Failed to allocate codec context\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0)
    {
        printf("Failed to copy codec parameters\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
    {
        printf("Failed to open codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVFrame *frame = av_frame_alloc();
    AVPacket packet;
    SwsContext *sws_ctx = nullptr;

    InferenceHandler handler(model_path, nullptr);
    if (!handler.initialize())
    {
        printf("Failed to initialize inference handler\n");
        av_frame_free(&frame);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

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

                    uint8_t *rgb_data[4];
                    int rgb_linesize[4];
                    av_image_alloc(rgb_data, rgb_linesize, frame->width, frame->height, AV_PIX_FMT_RGB24, 1);

                    if (!handler.runInference(rgb_data[0], frame->height, frame->width, 3))
                    {
                        printf("Failed to run inference\n");
                        av_freep(&rgb_data[0]);
                        break;
                    }

                    av_freep(&rgb_data[0]);
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);

    return 0;
}