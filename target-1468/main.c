#include "main.h"

#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "histo.h"

char hostname[HOST_NAME_MAX];
char do_all=1;

static struct option long_options[] = {
    {"input", required_argument, 0, 'i'},
    {"trials", required_argument, 0, 't'},
    {
        0,
        0,
        0,
        0,
    },
};

typedef void* (*Routine)(void*);

typedef struct {
    float orig_msec;
    float best_msec;
} TestPerformance;

typedef struct {
    int N;
    int B;
} HistogramTestParams;


void clear_3d(int Ni, int Nj, int Nk, float a[Ni][Nj][Nk]) {
    for (int i = 0; i < Ni; ++i) {
        for (int j = 0; j < Nj; ++j) {
            for (int k = 0; k < Nk; ++k) {
                a[i][j][k] = 0;
            }
        }
    }
}

void gen_3d(int Ni, int Nj, int Nk, float a[Ni][Nj][Nk]) {
    for (int i = 0; i < Ni; ++i) {
        for (int j = 0; j < Nj; ++j) {
            for (int k = 0; k < Nk; ++k) {
                a[i][j][k] = ((float)rand()) / (RAND_MAX / 8);
            }
        }
    }
}

void clear_1d(int N, int data[N]) {
    for (int i = 0; i < N; ++i) {
        data[i] = 0;
    }
}

void gen_1d(int N, int data[N]) {
    for (int i = 0; i < N; ++i) {
        data[i] = rand();
    }
}

static int max_errors_to_print = 5;

char check_3d(int Ni, int Nj, int Nk, float a[Ni][Nj][Nk],
              float a_check[Ni][Nj][Nk]) {
    int errors_printed = 0;
    char has_errors = 0;
    for (int i = 0; i < Ni; ++i) {
        for (int j = 0; j < Nj; ++j) {
            for (int k = 0; k < Nk; ++k) {
                if ((a[i][j][k] < (a_check[i][j][k] - 0.005)) ||
                    (a[i][j][k] > (a_check[i][j][k] + 0.005))) {
                    has_errors = 1;
                    if (errors_printed < max_errors_to_print) {
                        if (errors_printed == 0) printf("\n");
                        printf("Error on index: [%d][%d][%d].", i, j, k);
                        printf("Your output: %f, Correct output %f\n",
                               a[i][j][k], a_check[i][j][k]);
                        errors_printed++;
                    } else {
                        // printed too many errors already, just stop
                        if (max_errors_to_print != 0) {
                            printf("and many more errors likely exist...\n");
                        }
                        return 1;
                    }
                }
            }
        }
    }
    return has_errors;
}

char check_1d(int B, int a[B], int a_check[B]) {
    int errors_printed = 0;
    char has_errors = 0;
    for (int i = 0; i < B; ++i) {
        if (a[i] != a_check[i]) {
            has_errors = 1;
            if (errors_printed < max_errors_to_print) {
                if (errors_printed == 0) printf("\n");
                printf("Error on index: [%d]. ", i);
                printf("Your output: %d, Correct output %d\n", a[i],
                       a_check[i]);
                errors_printed++;
            } else {
                // printed too many errors already, just stop
                if (max_errors_to_print != 0) {
                    printf("and many more errors likely exist...\n");
                }
                return 1;
            }
        }
    }
    return has_errors;
}

void update_performance(uint64_t total_time, double computations,
                        TestPerformance* p) {
    double total_msec = total_time / 1000.0;

    float speedup = p->orig_msec / total_msec;
    p->best_msec = fmin(p->best_msec, total_msec);

    float ghz = 2.0;
    double clocks = total_time * 1000.0 * ghz;

    printf(" | %8.1f %6.3f %8.1f\n", total_msec, clocks/computations, speedup);
}

uint64_t spawn_threads_do_work(Routine func, void* argsPtrArray[NTHREADS]) {
    uint64_t start_time;
    pthread_t thread_id[NTHREADS];

    start_time = read_usec();

    int i;
    for (i = 0; i < NTHREADS; i++) {
        pthread_create(&thread_id[i], NULL, func, argsPtrArray[i]);
    }

    for (i = 0; i < NTHREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return (read_usec() - start_time);
}

float run_histogram_test(int i, HistogramTestParams* p, char check_func,
                         char* is_broken, TestPerformance* test_performance,
                         Routine func) {
    int N = p->N, B = p->B;
    printf("%2d %7s | %10d %10d", i,"Histo",N, B);
    fflush(stdout);

    void* data = malloc(sizeof(int) * N);
    void* hist = malloc(sizeof(int) * B);
    void* hist_check = 0;

    // Generate the inputs
    gen_1d(N, data);
    clear_1d(B, hist);

    if (check_func) {
        hist_check = calloc(sizeof(int), B);
        clear_1d(B, hist_check);
        histogram_check(N, B, data, hist_check);
    }

    HistogramArgs histArgsArray[NTHREADS];
    void* argsPtrArray[NTHREADS] = {0};
    for (int i = 0; i < NTHREADS; i++) {
        histArgsArray[i].data = (void*)data;
        histArgsArray[i].hist = (void*)hist;
        histArgsArray[i].id = i;

        argsPtrArray[i] = &histArgsArray[i];
    }

    uint64_t total_time = spawn_threads_do_work(func, argsPtrArray);

    if (check_func) {
        if (check_1d(B, hist, hist_check)) {
            *is_broken = 1;
        } else {
            //printf("no problems detected\n");
        }
    }

    update_performance(total_time, (double)N, test_performance);

    free(data);
    free(hist);
    if (hist_check) free(hist_check);

    return total_time;
}

#define NUM_HISTOGRAM_TESTS (2)
#define TOTAL_NUM_TESTS (NUM_HISTOGRAM_TESTS)

HistogramTestParams HistogramTests[NUM_HISTOGRAM_TESTS] = {{T1N, T1B},
                                                           {T2N, T2B}};

TestPerformance AllTestPerformance[TOTAL_NUM_TESTS] = {
    {200, 10000000.00}, {400, 10000000.00}};

char run_test(int i, char check_func) {
    char is_broken = 0;
    switch (i) {
        case 1:
            run_histogram_test(i,&(HistogramTests[0]), check_func, &is_broken,
                               &AllTestPerformance[i - 1],
                               compute_histogram_case1);
            break;
        case 2:
            run_histogram_test(i,&(HistogramTests[1]), check_func, &is_broken,
                               &AllTestPerformance[i - 1],
                               compute_histogram_case2);
            break;
        default:
            printf("WARNING! Expecting test case 1, or 2 \n");
            is_broken = 1;
            break;
    }
    return is_broken;
}

float interp(float s, float l, float lgrade, float h, float hgrade) {
    return (s - l) * (hgrade - lgrade) / (h - l) + lgrade;
}

float grade(float s) {
    if (s < 1) return 0;
    if (s < 3) return interp(s, 1, 0, 3, 100); // 1
    // higher than 4 gets Extra credit
    return 100;
}

int main(int argc, char** argv) {
    char check_func = 1;
    int test_case = 1; 
    int num_trials = 1;

    int result = gethostname(hostname, HOST_NAME_MAX);
    if (result) {
        perror("gethostname");
        return EXIT_FAILURE;
    }

    //Parse Inputs
    int opt;         
    while ((opt = getopt_long(argc, argv, "i:t:", long_options, 0)) != -1) {
      switch (opt) {
        case 'i':
          if(*optarg == 'a') {
            do_all=1;
          } else if(*optarg == 'g') { 
            do_all=2;
          } else {
            do_all=0;
            test_case=atoi(optarg);
          }
          break;
        case 't': 
          num_trials=atoi(optarg);
      }
    }

    reticulate_splines(do_all);

    if(strcmp(hostname,"lnxsrv07.seas.ucla.edu")!=0 && do_all!=2) {
      printf("Warning: The checker library may not work on machines other than lnxsrv07\n");
    }
  
    printf("\n");

    int benchmarks_failed = 0;

    printf("%2s %7s | %10s %10s | %8s %6s %8s\n",
          "T#", "Kernel",
          "Elements","Buckets", 
          "Time(ms)", "CPE", "Speedup");

    for (int t = 0; t < num_trials; ++t) {
        init();
        if (do_all) {
            for (int i = 1; i < TOTAL_NUM_TESTS + 1; i++) {
                benchmarks_failed += run_test(i, check_func);
            }
        } else {
            run_test(test_case, check_func);
        }
    }

    if (benchmarks_failed) {
        printf("Number of Benchmarks FAILED: %d\n", benchmarks_failed);
        printf("No grade given, because of incorrect execution.\n");
        return 0;
    }

    if (do_all) {
        float gmean_speedup = 1;
        for (int i = 0; i < TOTAL_NUM_TESTS; i++) {
            double speedup = ((double)AllTestPerformance[i].orig_msec /
                              (double)AllTestPerformance[i].best_msec);
            gmean_speedup *= speedup;
        }
        gmean_speedup = pow(gmean_speedup, 1.0 / TOTAL_NUM_TESTS);
        printf("Geomean Speedup: %0.2f\n", gmean_speedup);

        printf("Grade: %0.1f\n", grade(gmean_speedup));
        if(do_all==1){
          printf("The above grade is NOT AT ALL ACCURATE on lnxsrv07. Don't use this to performance-debug.\n");
          printf("By running \"make submit\", you will get your real grade.\n");
          printf("\"make submit\" will also update the scoreboard.\n");
        }
    } else {
        double speedup = ((double)AllTestPerformance[test_case - 1].orig_msec /
                          (double)AllTestPerformance[test_case - 1].best_msec);
        printf("Test Case %d Speedup: %0.2f\n", test_case, speedup);

        printf("No grade given, because only one test is run.\n");
        printf(" ... To see your grade, run all tests with \"-i a\" (or just no params)\n");
    }
    return 0;
}
