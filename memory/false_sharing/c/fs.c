#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #warning "on windows, replacing threads.h with windows-specific impl."

    #include <pthread.h>
    #include <stdint.h>

    #define thrd_t pthread_t
    #define thrd_success 0
    #define thrd_error 1
    typedef  int (*thrd_start_t) (void *);

    static inline int thrd_create(thrd_t *th, int (*run_t)(void *), void *arg) {
        return (pthread_create(th, NULL, (void *(*)(void *))(uintptr_t)run_t, arg) == 0) 
            ? thrd_success : thrd_error;
    }

    static inline int thrd_join(thrd_t th, int *res) {
        void *pth;
        if (pthread_join(th, &pth) != 0) return thrd_error;
        if (res) *res = (int)(intptr_t)pth;
        return thrd_success;
    }

#else
    #include <threads.h>
#endif

#include <unistd.h>
#include "measure.h"
#include "stats.h"
#define ARR_WIDTH 8 * 1024
#define ARR_SIZE (ARR_WIDTH * ARR_WIDTH)

struct thread_args {
    int   *n;
    float *arr;
    size_t count;
};

int verify(float *a, size_t i) {
    return a[i] > 511;
}

int BAD_run(void *arg) {
    struct thread_args *ta = (struct thread_args *)arg;
    for (size_t i = 0; i < ta->count; i++) {
        if (verify(ta->arr, i)) {
            *(ta->n) += 1;
        }
    }
    return 0;
}

int GOOD_run(void *arg) {
    struct thread_args *ta = (struct thread_args *)arg;
    int count = 0;
    for (size_t i = 0; i < ta->count; i++) {
        if (verify(ta->arr, i)) {
            count += 1;
        }
    }
    *(ta->n) = count;
    return count;
}


// OPTIONS
int use_multithreading = 0;
int use_good_solution  = 0;
int use_padding        = 0;
size_t num_of_threads  = 1;
size_t num_of_runs     = 32;
size_t byte_padding    = 1;

int process_options(int argc, char *argv[argc + 1]) {
    int opt;
    while ((opt = getopt(argc, argv, "mgt:r:p:b:")) != -1) {
        switch (opt) {
            case 'm':
                use_multithreading = 1;
                break;
            case 'g':
                use_multithreading = 1;
                use_good_solution = 1;
                break;
            case 't':
                use_multithreading = 1;
                num_of_threads = atoi(optarg);
                break;
            case 'r':
                num_of_runs = atoi(optarg);
                break;
            case 'p':
                use_padding = 1;
                break;
            case 'b':
                byte_padding = atoi(optarg);
                break;
            default:
                return -1;
        }
    }
    return 0;
}

// DATA
float arr[ARR_SIZE];

void init(void) {
    srand(42);
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = ((float) rand()) / RAND_MAX * ARR_WIDTH;
    }
}

typedef int (*run_t) (void);

int calculate(size_t num_of_threads, thrd_start_t fn) {
    const int part = ARR_SIZE / num_of_threads;
    int *counters = (int *) malloc(num_of_threads * sizeof(int) * byte_padding);
    struct thread_args *ta = (struct thread_args *) malloc(num_of_threads * sizeof(struct thread_args));
    thrd_t *t = (thrd_t *) malloc(num_of_threads * sizeof(thrd_t));

    for (size_t i = 0; i < num_of_threads; i++) {
        counters[i * byte_padding] = 0;
        // this prints addresses of counters and distance in bytes in between.
        // printf(
                // "counter %zu: %p (%ld)\n", 
                // i, 
                // (void *)(counters + i * byte_padding), 
                // (void *)(counters + i * byte_padding) - (void *)(counters)
        // );

        (ta + i)->n = (void *)(counters + i * byte_padding); 
        (ta + i)->arr = (void *)(arr + i*part); 
        (ta + i)->count = part; 

        thrd_create((t + i), fn, (void *)(ta + i));
    }

    int res = 0;
    for (size_t i = 0; i < num_of_threads; i++) {
        thrd_join(t[i], &res);
    }

    int count = 0;
    for (size_t i = 0; i < num_of_threads; i++) {
        count += counters[i];
    }

    free(counters);
    free(ta);
    free(t);

    return count;
}

int calculate_with_threads(void) {
    thrd_start_t fn;
    if (use_good_solution) {
        fn = GOOD_run;
    } else {
        fn = BAD_run;
    }
    return calculate(num_of_threads, fn);
}

int calculate_simple(void) {
    int count = 0;
    for (size_t i = 0; i < ARR_SIZE; i++) {
        if (verify(arr, i)) {
            count += 1;
        }
    }
    return count;
}

int main(int argc, char *argv[argc + 1]) {
    init();
    if (process_options(argc, argv) == -1) {
        fprintf(stderr, "usage: %s [-mgp] [-t count] [-r count]\n", argv[0]);
        return -1;
    }

    printf(
            "%s, threads: %zu, number of runs: %zu, padding: %zu bytes\n_\n", 
            use_multithreading ? 
                (use_good_solution ? "multi-threaded (GOOD)" : "multi-threaded (BAD)") : 
                "single-threaded",
            use_multithreading ? num_of_threads : 1,
            num_of_runs,
            (byte_padding * 4)
    );

    run_t action;
    if (use_multithreading) {
        action = calculate_with_threads;
    } else {
        action = calculate_simple;
    }

    struct stats st;
    EVAL(
            action(), 
            num_of_runs,
            st // statistics
    );

    printf(
           "avg: %2.2fms, min: %2.2fms, max: %2.2fms\np95: (%2.2fms - %2.2fms)\nstd: %2.2fms\n", 
            st.avg,
            st.min,
            st.max,
            st.p95min,
            st.p95max,
            st.deviation
    );

    return 0;
}
