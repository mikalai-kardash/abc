#include <math.h>
#include "stats.h"

void calculate_stats(float *results, int length, struct stats *st) {
    float min = 10.0;
    float max = 0.0;
    float sum = 0.0;

    for (int i = 0; i < length; i++) {
        min = (min > results[i]) ? results[i] : min;
        max = (max < results[i]) ? results[i] : max;
        sum += results[i];
    }

    float avg = sum / (float) length;
    float std = 0.0;
    for (int i = 0; i < length; i++) {
        float diff = results[i] - avg;
        std += diff * diff;
    }
    
    float deviation = sqrtf(std / length);

    float p95min = avg - deviation*2;
    p95min = (p95min < min) ? min : p95min;

    float p95max = avg + deviation*2;
    p95max = (p95max > max) ? max : p95max;

    st->avg = avg;
    st->min = min;
    st->max = max;
    st->deviation = deviation;
    st->p95max = p95max;
    st->p95min = p95min;
}
