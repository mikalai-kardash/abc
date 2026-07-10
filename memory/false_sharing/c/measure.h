#ifndef MEASURE_H
#define MEASURE_H

#include <time.h>
#include <stdlib.h>
#include "stats.h"

// source: https://stackoverflow.com/a/51974214
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define MS_PER_SEC      1000ULL     // MS = milliseconds
#define US_PER_MS       1000ULL     // US = microseconds
#define HNS_PER_US      10ULL       // HNS = hundred-nanoseconds (e.g., 1 hns = 100 ns)
#define NS_PER_US       1000ULL

#define HNS_PER_SEC     (MS_PER_SEC * US_PER_MS * HNS_PER_US)
#define NS_PER_HNS      (100ULL)    // NS = nanoseconds
#define NS_PER_SEC      (MS_PER_SEC * US_PER_MS * NS_PER_US)

int clock_gettime_monotonic(struct timespec *tv);
int clock_gettime(clockid_t type, struct timespec *tp);

#endif//defined(_WIN32) || defined(_WIN64)


#if defined(__linux__)
#include <bits/time.h>
#endif//defined(__linux__)

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

//
#define RUN(fn, results, n) {                               \
    for (size_t i = 0; i < n; i++) {                           \
        MEASURE(fn, results[i]);                            \
    }                                                       \
}

//
#define EVAL(fn, n, st) {                                   \
    float *results = malloc(n * sizeof(float));             \
    RUN(fn, results, n);                                    \
    calculate_stats(results, n, &st);                       \
    free(results);                                          \
}

#endif//MEASURE_H
