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

#include "camera_handler.h"
#include "inference_handler.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <rknn model>\n", argv[0]);
        return -1;
    }

    const char *model_path = argv[1];
    const char *camera_url = "video=0"; // Adjust this for your camera input.

    try
    {
        CameraHandler camera(camera_url);
        InferenceHandler handler(model_path, nullptr);

        if (!handler.initialize())
        {
            printf("Failed to initialize inference handler\n");
            return -1;
        }

        uint8_t *rgb_data = nullptr;
        int width, height, channels;

        while (camera.getNextFrame(&rgb_data, width, height, channels))
        {
            if (!handler.runInference(rgb_data, height, width, channels))
            {
                printf("Failed to run inference\n");
                av_freep(&rgb_data);
                break;

            }

            av_freep(&rgb_data);
        }
    }
    catch (const std::exception &e)
    {
        printf("Error: %s\n", e.what());
        return -1;
    }

    return 0;
}