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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define PERF_WITH_POST 1

InferenceHandler::InferenceHandler(const std::string &model_path, const std::string &image_path)
    : model_path_(model_path), image_path_(image_path), model_data_(nullptr),
      input_data_(nullptr), resize_buf_(nullptr), img_width_(0), img_height_(0), img_channel_(0),
      width_(0), height_(0), channel_(3), box_conf_threshold_(BOX_THRESH), nms_threshold_(NMS_THRESH)
{
    memset(&rga_ctx_, 0, sizeof(rga_context));
}

InferenceHandler::~InferenceHandler()
{
    releaseResources();
}

bool InferenceHandler::initialize()
{
    if (!loadModel())
        return false;

    if (rknn_init(&ctx_, model_data_, 0, 0) < 0)
    {
        printf("rknn_init failed\n");
        return false;
    }

    if (rknn_query(ctx_, RKNN_QUERY_IN_OUT_NUM, &io_num_, sizeof(io_num_)) < 0)
    {
        printf("rknn_query failed\n");
        return false;
    }

    input_attrs_ = new rknn_tensor_attr[io_num_.n_input];
    output_attrs_ = new rknn_tensor_attr[io_num_.n_output];
    memset(input_attrs_, 0, sizeof(rknn_tensor_attr) * io_num_.n_input);
    memset(output_attrs_, 0, sizeof(rknn_tensor_attr) * io_num_.n_output);

    for (int i = 0; i < io_num_.n_input; i++)
    {
        input_attrs_[i].index = i;
        if (rknn_query(ctx_, RKNN_QUERY_INPUT_ATTR, &input_attrs_[i], sizeof(rknn_tensor_attr)) < 0)
        {
            printf("rknn_query input attr failed\n");
            return false;
        }
    }

    for (int i = 0; i < io_num_.n_output; i++)
    {
        output_attrs_[i].index = i;
        if (rknn_query(ctx_, RKNN_QUERY_OUTPUT_ATTR, &output_attrs_[i], sizeof(rknn_tensor_attr)) < 0)
        {
            printf("rknn_query output attr failed\n");
            return false;
        }
        out_scales_.push_back(output_attrs_[i].scale);
        out_zps_.push_back(output_attrs_[i].zp);
    }

    if (input_attrs_[0].fmt == RKNN_TENSOR_NCHW)
    {
        width_ = input_attrs_[0].dims[0];
        height_ = input_attrs_[0].dims[1];
    }
    else
    {
        width_ = input_attrs_[0].dims[1];
        height_ = input_attrs_[0].dims[2];
    }

    RGA_init(&rga_ctx_);
    return true;
}

bool InferenceHandler::loadModel()
{
    int model_data_size = 0;
    model_data_ = load_model(model_path_.c_str(), &model_data_size);
    if (!model_data_)
    {
        printf("Failed to load model\n");
        return false;
    }
    return true;
}

bool InferenceHandler::prepareInput()
{
    input_data_ = load_image(image_path_.c_str(), &img_height_, &img_width_, &img_channel_, &input_attrs_[0]);
    if (!input_data_)
    {
        printf("Failed to load image\n");
        return false;
    }

    resize_buf_ = malloc(height_ * width_ * channel_);
    if (!resize_buf_)
    {
        printf("Failed to allocate resize buffer\n");
        return false;
    }

    rga_resize(&rga_ctx_, -1, input_data_, img_width_, img_height_, -1, resize_buf_, width_, height_);
    return true;
}

bool InferenceHandler::runInference(unsigned char *image_data, int height, int width, int channel)
{
    img_width_ = width;
    img_height_ = height;
    img_channel_ = channel;

    if (!image_data)
    {
        printf("Invalid image data\n");
        return false;
    }

    // Allocate resize buffer
    resize_buf_ = malloc(height_ * width_ * channel_);
    if (!resize_buf_)
    {
        printf("Failed to allocate resize buffer\n");
        return false;
    }

    // Resize the input image
    rga_resize(&rga_ctx_, -1, image_data, img_width_, img_height_, -1, resize_buf_, width_, height_);

    // Prepare input tensor
    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = width_ * height_ * channel_;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = resize_buf_;

    if (rknn_inputs_set(ctx_, io_num_.n_input, inputs) < 0)
    {
        printf("rknn_inputs_set failed\n");
        return false;
    }

    // Run inference
    if (rknn_run(ctx_, nullptr) < 0)
    {
        printf("rknn_run failed\n");
        return false;
    }

    // Get output tensors
    rknn_output outputs[io_num_.n_output];
    memset(outputs, 0, sizeof(outputs));
    for (int i = 0; i < io_num_.n_output; i++)
    {
        outputs[i].want_float = 0;
    }

    if (rknn_outputs_get(ctx_, io_num_.n_output, outputs, nullptr) < 0)
    {
        printf("rknn_outputs_get failed\n");
        return false;
    }

    // Post-process the results
    post_process((uint8_t *)outputs[0].buf, (uint8_t *)outputs[1].buf, (uint8_t *)outputs[2].buf, height_, width_,
                 box_conf_threshold_, nms_threshold_, (float)width_ / img_width_, (float)height_ / img_height_,
                 out_zps_, out_scales_, &detect_result_group_);

    // Release output tensors
    rknn_outputs_release(ctx_, io_num_.n_output, outputs);

    return true;
}

bool InferenceHandler::runInference()
{
    if (!prepareInput())
        return false;

    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = width_ * height_ * channel_;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = resize_buf_;

    if (rknn_inputs_set(ctx_, io_num_.n_input, inputs) < 0)
    {
        printf("rknn_inputs_set failed\n");
        return false;
    }

    if (rknn_run(ctx_, nullptr) < 0)
    {
        printf("rknn_run failed\n");
        return false;
    }

    rknn_output outputs[io_num_.n_output];
    memset(outputs, 0, sizeof(outputs));
    for (int i = 0; i < io_num_.n_output; i++)
    {
        outputs[i].want_float = 0;
    }

    if (rknn_outputs_get(ctx_, io_num_.n_output, outputs, nullptr) < 0)
    {
        printf("rknn_outputs_get failed\n");
        return false;
    }

    post_process((uint8_t *)outputs[0].buf, (uint8_t *)outputs[1].buf, (uint8_t *)outputs[2].buf, height_, width_,
                 box_conf_threshold_, nms_threshold_, (float)width_ / img_width_, (float)height_ / img_height_,
                 out_zps_, out_scales_, &detect_result_group_);

    rknn_outputs_release(ctx_, io_num_.n_output, outputs);
    return true;
}

void InferenceHandler::postProcessAndSave()
{
    CImg<unsigned char> img(image_path_.c_str());
    const unsigned char blue[] = {0, 0, 255};
    const unsigned char white[] = {255, 255, 255};
    char text[256];

    for (int i = 0; i < detect_result_group_.count; i++)
    {
        detect_result_t *det_result = &(detect_result_group_.results[i]);
        sprintf(text, "%s %.2f", det_result->name, det_result->prop);
        int x1 = det_result->box.left;
        int y1 = det_result->box.top;
        int x2 = det_result->box.right;
        int y2 = det_result->box.bottom;
        img.draw_rectangle(x1, y1, x2, y2, blue, 1, ~0U);
        img.draw_text(x1, y1 - 12, text, white);
    }
    img.save("./out.bmp");
}

void InferenceHandler::releaseResources()
{
    if (ctx_)
        rknn_destroy(ctx_);
    if (model_data_)
        free(model_data_);
    if (input_data_)
        free(input_data_);
    if (resize_buf_)
        free(resize_buf_);
    if (input_attrs_)
        delete[] input_attrs_;
    if (output_attrs_)
        delete[] output_attrs_;
    RGA_deinit(&rga_ctx_);
}