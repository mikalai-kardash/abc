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

void calculate_stats(float *results, int length, struct stats *st);

#endif//STATS_H
