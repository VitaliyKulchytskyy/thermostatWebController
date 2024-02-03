#include "utils.h"


int atoi_s(const char* value, int min, int replace) {
    int interval = atoi(value);
    return interval > min ? interval : replace;
}

float atof_s(const char* value, float min, float replace) {
    float interval = atof(value);
    return interval > min ? interval : replace;
}