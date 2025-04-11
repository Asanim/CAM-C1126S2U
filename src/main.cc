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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <rknn model> <bmp>\n", argv[0]);
        return -1;
    }

    InferenceHandler handler(argv[1], argv[2]);

    if (!handler.initialize())
    {
        printf("Failed to initialize inference handler\n");
        return -1;
    }

    if (!handler.runInference())
    {
        printf("Failed to run inference\n");
        return -1;
    }

    handler.postProcessAndSave();
    return 0;
}