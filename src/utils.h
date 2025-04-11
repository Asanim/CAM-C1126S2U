#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <vector>
#include "rknn_api.h"
#include "postprocess.h"
#include "rga_func.h"
#include "CImg/CImg.h"

using namespace cimg_library;

class Utils {
public:
    static unsigned char* load_model(const char* filename, int* model_size);
    static unsigned char* load_image(const char* image_path, int* org_height, int* org_width, int* org_ch, rknn_tensor_attr* input_attr);
    static int save_float(const char* file_name, float* output, int element_size);
    static void dump_tensor_attr(rknn_tensor_attr* attr);
    static double get_us(struct timeval t);
    static void draw_objects(CImg<unsigned char>& img, detect_result_group_t& detect_result_group, int img_width, int img_height);
};

#endif // UTILS_H