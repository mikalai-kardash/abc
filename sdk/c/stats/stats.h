#ifndef STATS_H
#define STATS_H

struct stats {
    float avg;
    float min;
    float max;
    float deviation;
    float p95min;
    float p95max;
};

#ifdef __cplusplus
extern "C" {
#endif

void calculate_stats(float *results, int length, struct stats *st);

#ifdef __cplusplus
}
#endif
#endif//STATS_H
