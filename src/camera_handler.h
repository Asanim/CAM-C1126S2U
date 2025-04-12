#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
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
    cv::VideoCapture video_capture_; // OpenCV video capture object
};

#endif // CAMERA_HANDLER_H
