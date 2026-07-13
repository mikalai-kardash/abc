#include "measure.h"

#if defined(_WIN32) || defined(_WIN64)

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
