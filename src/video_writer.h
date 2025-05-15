#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <stdexcept>
#include <vector>
#include <string>

class CameraHandler
{
public:
    CameraHandler(const char *camera_url);
    ~CameraHandler();

    bool getNextFrame(uint8_t **rgb_data, int &width, int &height, int &channels);

    static std::vector<std::string> listAvailableCameras();

private:
    AVFormatContext *format_ctx;
    AVCodecContext *codec_ctx;
    SwsContext *sws_ctx;
    AVFrame *frame;
    int video_stream_index;
};

#endif // CAMERA_HANDLER_H
