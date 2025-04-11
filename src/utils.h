#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "rknn_api.h"

unsigned char *load_image(const char *image_path, int *org_height, int *org_width, int *org_ch, rknn_tensor_attr *input_attr);

unsigned char *load_model(const char *filename, int *model_size);

void dump_tensor_attr(rknn_tensor_attr *attr);

// Function prototypes
static int saveFloat(const char *file_name, float *output, int element_size);

static unsigned char *load_data(FILE *fp, size_t ofst, size_t sz);

inline const char *get_type_string(rknn_tensor_type type);

inline const char *get_qnt_type_string(rknn_tensor_qnt_type type);

inline const char *get_format_string(rknn_tensor_format fmt);

double __get_us(struct timeval t);

#endif // UTILS_H