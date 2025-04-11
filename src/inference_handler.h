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

#ifndef INFERENCE_HANDLER_H
#define INFERENCE_HANDLER_H

#include "rga_func.h"
#include "rknn_api.h"
#include "postprocess.h"
#include "utils.h"

#undef cimg_display
#define cimg_display 0
#include "CImg/CImg.h"

#include <vector>
#include <string>
#include <sys/time.h>

using namespace cimg_library;

class InferenceHandler
{
public:
    InferenceHandler(const std::string &model_path, const std::string &image_path);
    ~InferenceHandler();

    bool initialize();
    bool runInference();
    void postProcessAndSave();

private:
    std::string model_path_;
    std::string image_path_;
    rknn_context ctx_;
    rga_context rga_ctx_;
    rknn_input_output_num io_num_;
    rknn_tensor_attr *input_attrs_;
    rknn_tensor_attr *output_attrs_;
    unsigned char *model_data_;
    unsigned char *input_data_;
    void *resize_buf_;
    int img_width_, img_height_, img_channel_;
    int width_, height_, channel_;
    float box_conf_threshold_;
    float nms_threshold_;
    std::vector<float> out_scales_;
    std::vector<uint32_t> out_zps_;
    detect_result_group_t detect_result_group_;

    bool loadModel();
    bool prepareInput();
    void releaseResources();
};

#endif // INFERENCE_HANDLER_H