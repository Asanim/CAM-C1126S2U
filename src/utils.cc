#include "utils.h"
#include "stb/stb_image.h"

unsigned char* Utils::load_model(const char* filename, int* model_size) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Open file %s failed.\n", filename);
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char* data = (unsigned char*)malloc(size);
    if (!data) {
        printf("Buffer malloc failure.\n");
        fclose(fp);
        return nullptr;
    }

    fread(data, 1, size, fp);
    fclose(fp);

    *model_size = size;
    return data;
}

unsigned char* Utils::load_image(const char* image_path, int* org_height, int* org_width, int* org_ch, rknn_tensor_attr* input_attr) {
    int req_height = 0, req_width = 0, req_channel = 0;

    switch (input_attr->fmt) {
        case RKNN_TENSOR_NHWC:
            req_height = input_attr->dims[2];
            req_width = input_attr->dims[1];
            req_channel = input_attr->dims[0];
            break;
        case RKNN_TENSOR_NCHW:
            req_height = input_attr->dims[1];
            req_width = input_attr->dims[0];
            req_channel = input_attr->dims[2];
            break;
        default:
            printf("Unsupported layout\n");
            return nullptr;
    }

    int height = 0, width = 0, channel = 0;
    unsigned char* image_data = stbi_load(image_path, &width, &height, &channel, req_channel);
    if (!image_data) {
        printf("Load image failed!\n");
        return nullptr;
    }

    *org_width = width;
    *org_height = height;
    *org_ch = channel;

    return image_data;
}

int Utils::save_float(const char* file_name, float* output, int element_size) {
    FILE* fp = fopen(file_name, "w");
    if (!fp) {
        printf("Failed to open file %s for writing.\n", file_name);
        return -1;
    }

    for (int i = 0; i < element_size; i++) {
        fprintf(fp, "%.6f\n", output[i]);
    }

    fclose(fp);
    return 0;
}

void Utils::dump_tensor_attr(rknn_tensor_attr* attr) {
    printf("  index=%d, name=%s, n_dims=%d, dims=[%d, %d, %d, %d], n_elems=%d, size=%d, fmt=%s, type=%s, qnt_type=%s, "
           "zp=%d, scale=%f\n",
           attr->index, attr->name, attr->n_dims, attr->dims[3], attr->dims[2], attr->dims[1], attr->dims[0],
           attr->n_elems, attr->size, get_format_string(attr->fmt), get_type_string(attr->type),
           get_qnt_type_string(attr->qnt_type), attr->zp, attr->scale);
}

double Utils::get_us(struct timeval t) {
    return (t.tv_sec * 1000000 + t.tv_usec);
}

void Utils::draw_objects(CImg<unsigned char>& img, detect_result_group_t& detect_result_group, int img_width, int img_height) {
    const unsigned char blue[] = {0, 0, 255};
    const unsigned char white[] = {255, 255, 255};
    char text[256];

    for (int i = 0; i < detect_result_group.count; i++) {
        detect_result_t* det_result = &(detect_result_group.results[i]);
        sprintf(text, "%s %.2f", det_result->name, det_result->prop);
        printf("%s @ (%d %d %d %d) %f\n",
               det_result->name,
               det_result->box.left, det_result->box.top, det_result->box.right, det_result->box.bottom,
               det_result->prop);

        int x1 = det_result->box.left;
        int y1 = det_result->box.top;
        int x2 = det_result->box.right;
        int y2 = det_result->box.bottom;

        img.draw_rectangle(x1, y1, x2, y2, blue, 1, ~0U);
        img.draw_text(x1, y1 - 12, text, white);
    }
}