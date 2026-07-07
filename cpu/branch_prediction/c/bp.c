#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "measure.h"
#include "stats.h"
#define DATA_SIZE 1024 * 1024
#define LIMIT (float) DATA_SIZE / 2 - 1

float a[DATA_SIZE];

int use_random = 0;
int number_of_runs = 128;

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
    while ((opt = getopt(argc, argv, "ron:")) != -1) {
        switch (opt) {
            case 'r':
                use_random = 1;
                break;

            case 'o':
                use_random = 0;
                break;

            case 'n':
                number_of_runs = atoi(optarg);
                break;

            default:
                return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[argc + 1]) {
    if (process_options(argc, argv) < 0) {
        fprintf(stderr, "usage: %s [-ro] [-n count]\n", argv[0]);
        return 1;
    }
    init(a);

    float *results = (float *) malloc(number_of_runs * sizeof(float));
    RUN(
            calculate(a, LIMIT, DATA_SIZE), 
            results, number_of_runs 
    );

    struct stats st;
    calculate_stats(results, number_of_runs, &st);

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
