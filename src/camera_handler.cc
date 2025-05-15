#include "camera_handler.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>
#include <vector>
#include <string>

CameraHandler::CameraHandler(const char *camera_url) : video_capture_(0)
{
    if (!video_capture_.isOpened())
    {
        throw std::runtime_error("Failed to open camera");
    }
}

CameraHandler::~CameraHandler()
{
    video_capture_.release();
}

bool CameraHandler::getNextFrame(uint8_t **rgb_data, int &width, int &height, int &channels)
{
    cv::Mat frame;
    if (video_capture_.read(frame))
    {
        width = frame.cols;
        height = frame.rows;
        channels = frame.channels();

        size_t data_size = width * height * channels;
        *rgb_data = new uint8_t[data_size];
        std::memcpy(*rgb_data, frame.data, data_size);

        return true;
    }
    return false;
}

std::vector<std::string> CameraHandler::listAvailableCameras()
{
    std::vector<std::string> availableCameras;
    // OpenCV does not provide a direct way to list cameras.
    // This can be implemented using platform-specific APIs if needed.
    return availableCameras;
}