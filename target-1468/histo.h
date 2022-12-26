#ifndef HISTO_H
#define HISTO_H

#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 8

#define T1N 100000000
#define T1B 8

#define T2N 25000000
#define T2B 16000000

typedef struct {
    void* data;
    void* hist;
    int id;
} HistogramArgs;

void init();

void histogram_check(int N, int B, const int data[N], int hist[B]);

void* compute_histogram_case1(void* input);
void* compute_histogram_case2(void* input);

void reticulate_splines(int do_all);

#endif
