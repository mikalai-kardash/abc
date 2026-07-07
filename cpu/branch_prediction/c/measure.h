#ifndef MEASURE_H
#define MEASURE_H

#include <time.h>

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

int clock_gettime_monotonic(struct timespec *tv) {
    static LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER ticks;

    if (!ticksPerSec.QuadPart) {
        QueryPerformanceFrequency(&ticksPerSec);
        if (!ticksPerSec.QuadPart) {
            errno = ENOTSUP;
            return -1;
        }
    }

    QueryPerformanceCounter(&ticks);

    tv->tv_sec = (long) (ticks.QuadPart / ticksPerSec.QuadPart);
    tv->tv_nsec = (long) (((ticks.QuadPart % ticksPerSec.QuadPart) * NS_PER_SEC) / ticksPerSec.QuadPart);

    return 0;
}

int clock_gettime(clockid_t type, struct timespec *tp) {
    if (type == CLOCK_MONOTONIC) {
        return clock_gettime_monotonic(tp);
    }

    errno = ENOTSUP;
    return -1;
}
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

#define RUN(fn, results) {              \
    for (int i = 0; i < 128; i++) {     \
        float elapsed_ms;               \
        MEASURE(fn, elapsed_ms);        \
        results[i] = elapsed_ms;        \
    }                                   \
}

#endif//MEASURE_H
