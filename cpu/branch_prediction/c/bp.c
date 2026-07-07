#include <bits/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define DATA_SIZE 1024 * 1024
#define LIMIT (float) DATA_SIZE / 2 - 1

float a[DATA_SIZE];

float calculate(float *a, float limit, int length) {
    float sum = 0.0;
    for (int i = 0; i < length; i++) {
        if (a[i] < limit) {
            sum += a[i];
        }
    }
    return sum;
}

void init_randomized(float *a, int length) {
    srand(42);
    for (int i = 0; i < length; i++) {
        a[i] = ((float) rand()) / RAND_MAX * DATA_SIZE;
    }
}

void init_ordered(float *a, int length) {
    for (int i = 0; i < length; i++) {
        a[i] = (float) i;
    }
}

#define MEASURE(fn, elapsed) {                              \
    struct timespec start, end;                             \
    clock_gettime(CLOCK_MONOTONIC, &start);                 \
    fn;                                                     \
    clock_gettime(CLOCK_MONOTONIC, &end);                   \
    elapsed =                                               \
        (end.tv_sec - start.tv_sec) * 1000.0 +              \
        (end.tv_nsec - start.tv_nsec) / (1000.0 * 1000.0)   \
    ;                                                       \
}

struct stats {
    float avg;
    float min;
    float max;
    float deviation;
    float p95min;
    float p95max;
};

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
    
    float deviation = sqrtf(std / 128);

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

#define RUN(fn, results) {              \
    for (int i = 0; i < 128; i++) {     \
        float elapsed_ms;               \
        MEASURE(fn, elapsed_ms);        \
        results[i] = elapsed_ms;        \
    }                                   \
}


int use_random = 0;

void init(float *a) {
    if (use_random) {
        init_randomized(a, DATA_SIZE);
    } else {
        init_ordered(a, DATA_SIZE);
    }
}

int process_options(int argc, char *argv[argc + 1]) {
    if (argc < 2) {
        return -1;
    }

    int opt;
    while ((opt = getopt(argc, argv, "ro")) != -1) {
        switch (opt) {
            case 'r':
                use_random = 1;
                break;

            case 'o':
                use_random = 0;
                break;

            default:
                return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[argc + 1]) {
    if (process_options(argc, argv) < 0) {
        fprintf(stderr, "usage: %s [-ro]\n", argv[0]);
        return 1;
    }
    init(a);

    float results[128];
    RUN(calculate(a, LIMIT, DATA_SIZE), results);

    struct stats st;
    calculate_stats(results, 128, &st);

    printf(
           "DONE!\navg: %2.2fms, min: %2.2fms, max: %2.2fms\np95: (%2.2fms - %2.2fms)\nstd: %2.2fms\n", 
            st.avg,
            st.min,
            st.max,
            st.p95min,
            st.p95max,
            st.deviation
    );

    return 0;
}
