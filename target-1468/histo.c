#include "histo.h"

pthread_barrier_t barrier;

void init()
{
    pthread_barrier_init(&barrier, NULL, 8);
}

// // /************ Test Case 1 ************/

void* compute_histogram_case1(void* input) {

    HistogramArgs* histArgs = (HistogramArgs*)input;
    int* data = (int*)(histArgs->data);
    int* hist = (int*)(histArgs->hist);
    const int thread_id = histArgs->id;

    const int STEP = T1N / NTHREADS;
    const int start = thread_id * STEP;
    const int end = start + STEP;

    int temp[8] = {0};

    for (int i = start; i < end; i++) {
        temp[data[i]%T1B]++;          
    }

       __sync_fetch_and_add(hist, temp[0]);
    __sync_fetch_and_add(hist+1, temp[1]);
    __sync_fetch_and_add(hist+2, temp[2]);
    __sync_fetch_and_add(hist+3, temp[3]);
    __sync_fetch_and_add(hist+4, temp[4]);
    __sync_fetch_and_add(hist+5, temp[5]);
    __sync_fetch_and_add(hist+6, temp[6]);
    __sync_fetch_and_add(hist+7, temp[7]);

    return NULL;
}       


// /************ Test Case 2 ************/

char temp1[T2B] = {0};
char temp2[T2B] = {0};
char temp3[T2B] = {0};
char temp4[T2B] = {0};
char temp5[T2B] = {0};
char temp6[T2B] = {0};
char temp7[T2B] = {0};
char temp0[T2B] = {0};

void* compute_histogram_case2(void* input) {

    HistogramArgs* histArgs = (HistogramArgs*)input;
    int* data = (int*)(histArgs->data);
    int* hist = (int*)(histArgs->hist);
    const int thread_id = histArgs->id;

    int STEP = T2N / NTHREADS;
    int start = thread_id * STEP;
    int end = start + STEP;

    // let each thread take care of a portion of data into own arrays
    switch(thread_id)
    {
        case 0:
            for (int i = start; i < end; i++) {
                temp0[data[i]%T2B]++;          
                }
            break;
        case 1:
            for (int i = start; i < end; i++) {
                temp1[data[i]%T2B]++;          
                }
            break;
        case 2:
            for (int i = start; i < end; i++) {
                temp2[data[i]%T2B]++;          
                }
            break;
        case 3:
            for (int i = start; i < end; i++) {
                temp3[data[i]%T2B]++;          
                }
            break;
        case 4:
            for (int i = start; i < end; i++) {
                temp4[data[i]%T2B]++;          
                }
            break;
        case 5:
            for (int i = start; i < end; i++) {
                temp5[data[i]%T2B]++;          
                }
            break;
        case 6:
            for (int i = start; i < end; i++) {
                temp6[data[i]%T2B]++;          
                }
            break;
        case 7:
            for (int i = start; i < end; i++) {
                temp7[data[i]%T2B]++;          
                }
            break;
    }

    pthread_barrier_wait(&barrier);     // synchronize

    STEP = T2B / NTHREADS;
    start = thread_id * STEP;
    end = start + STEP;

    // let each thread take care of a portion of summing sep arrays into main array

    for (int i = start; i < end; i++) {
        hist[i] += temp0[i] + temp1[i] + temp2[i] + temp3[i] + temp4[i] + temp5[i] + temp6[i] + temp7[i];
    }

    return NULL;
    
}


// void init()
// {
//     pthread_barrier_init(&barrier, NULL, 16);

//     pthread_t thread_id[16];

//     for (long i = 8 ; i < 16 ; i++)
//     {
//         pthread_create(&thread_id[i], NULL, compute_histogram_case2, (void*) i);
//     }

//     for (long i = 8 ; i < 16 ; i++)
//     {
//         pthread_detach(thread_id[i]);
//     }
// }

// // /************ Test Case 1 ************/

// volatile static int (*ptr[8]) [8];

// void* compute_histogram_case1(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     const int STEP = T1N / NTHREADS;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};
//     ptr[thread_id] = &temp;

//     for (int i = start; i < end; i++) {
//         temp[data[i]%T1B]++;          
//     }

//     // have each handle reduction of one bucket, no contention

//     pthread_barrier_wait(&barrier);         // (wait on 8) added sync cost, reduced parallel (make sure ptrs are set and temps are loaded)

//     int histreduce = 0;

//     for (int i = 0; i < 8 ; i++)
//     {
//         histreduce += (*ptr[i])[thread_id]; // dereference pointer at ith pos and access thread_id-th element in array
//             /* did testing on local that this is correct behavior on Xcode*/
//     }

//      hist[thread_id] += histreduce;

//     // would previously use atomic ops to reduce all of one temp[8] into all of hist, each thread for itself.

//     pthread_barrier_wait(&barrier);

//     return NULL;
// }       


/************ Test Case 2 ************/

// static char temp1[T2B] = {0};
// static char temp2[T2B] = {0};
// static char temp3[T2B] = {0};
// static char temp4[T2B] = {0};
// static char temp5[T2B] = {0};
// static char temp6[T2B] = {0};
// static char temp7[T2B] = {0};
// static char temp0[T2B] = {0};

// static char temp15[T2B] = {0};
// static char temp14[T2B] = {0};
// static char temp13[T2B] = {0};
// static char temp12[T2B] = {0};
// static char temp11[T2B] = {0};
// static char temp10[T2B] = {0};
// static char temp9[T2B] = {0};
// static char temp8[T2B] = {0};

// static volatile int* globaldata;
// static volatile int* globalhist;

// void* compute_histogram_case2(void* input) {

//     long seperator = (long) input;          // separates mine from professor's
//     int thread_id;                          // dec for all

//     if (seperator >= 8 && seperator <= 15)
//     {
//         thread_id = seperator;                      // sets for 8 - 15 inclusive
//     }

//     int* data = NULL;
//     int* hist = NULL;                       //  declares

//     pthread_barrier_wait(&barrier);         // all 16

//     if (! (seperator >= 8 && seperator <= 15))          // not mine?
//     {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     data = (int*)(histArgs->data);
//     hist = (int*)(histArgs->hist);
//     thread_id = histArgs->id;

//     if (thread_id == 0)
//     {
//         globaldata = data;
//         globalhist = hist;
//     }                                           // one sets globals, rest take care of themselves

//     }

//     pthread_barrier_wait(&barrier);

//     if (thread_id >= 8 && thread_id <= 15)                  // 8 - 15 inclusive (mine)
//     {
//         data = globaldata;
//         hist = globalhist;                      // grab data and hist from global that was set ... volatile (in-memory, no caching in registers)
//     }

//     int STEP = T2N / 16;              // 1562500
//     int start = thread_id * STEP;
//     int end = start + STEP;

//     // let each thread take care of a portion of data into own arrays
//     switch(thread_id)
//     {
//         case 0:
//             for (int i = start; i < end; i++) {     // thread 
//                 temp0[data[i]%T2B]++;          
//                 }
//             break;
//         case 1:
//             for (int i = start; i < end; i++) {     // 
//                 temp1[data[i]%T2B]++;          
//                 }
//             break;
//         case 2:
//             for (int i = start; i < end; i++) {     //
//                 temp2[data[i]%T2B]++;          
//                 }
//             break;
//         case 3:
//             for (int i = start; i < end; i++) {
//                 temp3[data[i]%T2B]++;          
//                 }
//             break;
//         case 4:
//             for (int i = start; i < end; i++) {
//                 temp4[data[i]%T2B]++;          
//                 }
//             break;
//         case 5:
//             for (int i = start; i < end; i++) {
//                 temp5[data[i]%T2B]++;          
//                 }
//             break;
//         case 6:
//             for (int i = start; i < end; i++) {
//                 temp6[data[i]%T2B]++;          
//                 }
//             break;
//         case 7:
//             for (int i = start; i < end; i++) {
//                 temp7[data[i]%T2B]++;          
//                 }
//             break;
//         case 8:
//             for (int i = start; i < end; i++) {
//                 temp8[data[i]%T2B]++;          
//                 }
//             break;
//         case 9:
//             for (int i = start; i < end; i++) {
//                 temp9[data[i]%T2B]++;          
//                 }
//             break;
//         case 10:
//             for (int i = start; i < end; i++) {
//                 temp10[data[i]%T2B]++;          
//                 }
//             break;
//         case 11:
//             for (int i = start; i < end; i++) {
//                 temp11[data[i]%T2B]++;          
//                 }
//             break;
//         case 12:
//             for (int i = start; i < end; i++) {
//                 temp12[data[i]%T2B]++;          
//                 }
//             break;
//         case 13:
//             for (int i = start; i < end; i++) {
//                 temp13[data[i]%T2B]++;          
//                 }
//             break;
//         case 14:
//             for (int i = start; i < end; i++) {
//                 temp14[data[i]%T2B]++;          
//                 }
//             break;
//         case 15:
//             for (int i = start; i < end; i++) {
//                 temp15[data[i]%T2B]++;          
//                 }
//             break;
//     }

//     pthread_barrier_wait(&barrier);     // synchronize

//     STEP = T2B / 16;
//     start = thread_id * STEP;
//     end = start + STEP;

//     // let each thread take care of a portion of summing sep arrays into main array

//     int sum1 = 0;
//     int sum2 = 0;

//     for (int i = start; i < end; i++) {
//         sum1 = temp0[i] + temp1[i] + temp2[i] + temp3[i] + temp4[i] + temp5[i] + temp6[i] + temp7[i];
//         sum2 = temp8[i] + temp9[i] + temp10[i] + temp11[i] + temp12[i] + temp13[i] + temp14[i] + temp15[i];
//         hist[i] += sum1 + sum2;
//     }

//     return NULL;
    
// }












    // int temp_1 = 0;
    // int temp_2 = 0;

      // temp_1 = temp0[i] + temp1[i] + temp2[i] + temp3[i];
        // temp_2 = temp4[i] + temp5[i] + temp6[i] + temp7[i];
        // hist[i] += temp_1 + temp_2;


// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     if (thread_id == 0) {
//         for (int j = 0; j < T2N; j++) {
//             hist[data[j] % T2B]++;
//         }
//     }
    
//     return NULL;
    
// }

// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     const int STEP = T2N / NTHREADS;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int pos = 0;

//     for (int i = start; i < end; i++) {
        
//         pos = data[i]%T2B;
//         sem_wait(&semaphore[pos]);
//         hist[pos]++;    
//         sem_post(&semaphore[pos]);

//     }

//     return NULL;
    
// }

/* dat new new */
// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     // if (thread_id == 0) {
//     //     for (int j = 0; j < T2N; j++) {
//     //         hist[data[j] % T2B]++;
//     //     }
//     // }

//     // const int STEP = T2B/NTHREADS;
//     // const int start = thread_id * STEP;
//     // const int end = start + STEP;

//     // int current;

//     // for (int i = 0 ; i < T2N ; i ++)
//     // {
//     //     current = data[i]%T2B;      // bucket #

//     //     if (current >= start && current < end)      // within my range?
//     //     {
//     //         hist[current]++;
//     //     }
//     // }

//     int STEP = T2N / NTHREADS;
//     int start = thread_id * STEP;
//     int end = start + STEP;

//     int current;

//     for (int i = start; i < end ; i++)
//     {
//         current = data[i]%T2B;
//         __sync_fetch_and_add(hist+current, 1);
//     }
    
//     return NULL;
    
// }

// #include <semaphore.h>


// int* globaldata;
// int* globalhist;
// pthread_barrier_t barrier;
// pthread_barrier_t barr;
// sem_t semaphore;
// volatile int suspendedbuddies = 1;
// // volatile int suspended = 1;

// void* helper(void* input)
// {

//     const long thread_id = (long) input;

//     const int STEP = T1N / 32;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     while(suspendedbuddies){}

//     int* data = globaldata;
//     int* hist = globalhist;

//     for (int i = start; i < end; i++) {
//         temp[data[i]%T1B]++;          
//     }
    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }

//     pthread_barrier_wait(&barr);

//     if (thread_id == 8)
//     {
//         sem_post(&semaphore);
//         //  __sync_val_compare_and_swap(&suspended, 1, 0);
//     }
//     return NULL;
// }

// void init()
// {

// sem_init(&semaphore, 0, 0);
// pthread_barrier_init(&barrier, NULL, 8);
// pthread_barrier_init(&barr, NULL, 24);

//     pthread_t thread_id[32];

//     for (long i = 8 ; i < 32 ; i++)
//     {
//         pthread_create(&thread_id[i], NULL, helper, (void*)i);
//     }

//     for (long i = 8 ; i < 32 ; i++)
//         pthread_detach(thread_id[i]);
// }

// void* compute_histogram_case1(void* input)
// {
//     HistogramArgs* histArgs = (HistogramArgs*) input;
//     const int thread_id = histArgs->id;

//     if (thread_id == 0)
//     {
//         globaldata = histArgs->data;
//         globalhist = histArgs->hist;
//         __sync_val_compare_and_swap(&suspendedbuddies, 1, 0);
//     }

//     int* data = (int*) (histArgs->data);
//     int* hist = (int*) (histArgs->hist);

//     const int STEP = T1N/32;
//    const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i]%T1B]++;          
//     }
    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }

//     if (thread_id == 0)
//     {
//         sem_wait(&semaphore);
//     }

//     return NULL;
// }

// crap: 

    // for (int i = thread_id; i < T1N; i+=NTHREADS) {
    //     temp[data[i] % T1B]++;               
    // }
    
    // for (int i = 0 ; i < 8 ; i++)
    // {
    //     __sync_fetch_and_add(hist+i, temp[i]);
    // }

        // for (int i = 0 ; i < T2B; i++)
    // {
    //     sem_init(&semaphore[i], 0, 0);
    // }

// sem_t semaphore[T2B];

// exploit knowledge of how atomics actually work ... re-execute if overlapping exec. -> consistent 8.2 - 8.6
    // switch(thread_id)
    // {
    //     case 0:
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // break;
    //     case 1:
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // break;
    //     case 2:
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // break;
    //     case 3:
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // break;
    //     case 4:
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // break;
    //     case 5:
    // __sync_fetch_and_add(hist+5, temp[5]);
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // break;
    //     case 6:
    // __sync_fetch_and_add(hist+6, temp[6]);
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    // break;
    //     case 7:
    // __sync_fetch_and_add(hist+7, temp[7]);
    // __sync_fetch_and_add(hist, temp[0]);
    // __sync_fetch_and_add(hist+1, temp[1]);
    // __sync_fetch_and_add(hist+2, temp[2]);
    // __sync_fetch_and_add(hist+3, temp[3]);
    // __sync_fetch_and_add(hist+4, temp[4]);
    // __sync_fetch_and_add(hist+5, temp[5]);
    //  __sync_fetch_and_add(hist+6, temp[6]);
    // }





















// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     const int min = thread_id * 3125000;
//     const int max = (thread_id * 3125000) + 3125000;

//    for (int i = 0 ; i < T2N; i ++){
//     if ((data[i]%T2B) >= min && (data[i]%T2B) < max)
//         hist[data[i]%T2B]++;
//    }


//     return NULL;

// }









/* MY SEGFAULT CASE WITH GLOBAL VAR = HEX */
/* 

int* globaldata_case1 = (int*) 0x7fffdf776010;
int* globalhist_case1 = (int*) 0x604010;

sem_t semaphore;
pthread_barrier_t barrier;

void* helper(void*input)
{
    int* data = globaldata_case1;
    int* hist = globalhist_case1;

    const long thread_id = (long) input;

    const int STEP = T1N / 24;
    const int start = thread_id * STEP;
    const int end = start + STEP;

    int temp[8] = {0};

    for (int i = start; i < end; i++) {
        temp[data[i]%T1B]++;          
    }

    // for (int i = thread_id; i < T1N; i+=NTHREADS) {
    //     temp[data[i] % T1B]++;               
    // }

    
    for (int i = 0 ; i < 8 ; i++)
    {
        __sync_fetch_and_add( hist+i, temp[i]);
    }

    pthread_barrier_wait(&barrier);
    if (thread_id == 0)
    {
        sem_post(&semaphore);
    }
    return NULL;
}

void init()
{

sem_init(&semaphore, 0, 0);
pthread_barrier_init(&barrier, NULL, 24);

pthread_t thread_id[24];

 for (long i = 0 ; i < 24 ; i++)
    {
        pthread_create(&thread_id[i], NULL, helper, (void*)i);
    }

    for (size_t i = 0 ; i < 24 ; i++)
        pthread_detach(thread_id[i]);
}

void* compute_histogram_case1(void* input)
{
    HistogramArgs* histArgs = (HistogramArgs*) input;
    const int thread_id = histArgs->id;

    if (thread_id == 0)
    {
        sem_wait(&semaphore);
    }

    return NULL;
}

*/

// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);

//     const int thread_id = histArgs->id;

//     const int STEP = T2N / NTHREADS;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[T2B] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T2B]++;               
//     }

    
//     for (int i = 0 ; i < T2B ; i++)
//     {
//             __sync_fetch_and_add( hist+i, temp[i]);
//     }

//     return NULL;
// }   




// void* compute_histogram_case2(void* input) {

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);
//     const int thread_id = histArgs->id;

//     // if (thread_id == 0) {
//     //     for (int j = 0; j < T2N; j++) {
//     //         hist[data[j] % T2B]++;
//     //     }
//     // }

//     for (int i = thread_id ; i < T2N ; i+=NTHREADS)
//          __sync_fetch_and_add(hist + (data[i]%T2B), 1);

//     return NULL;
// }

/* I GOT IT TO WORK */

// sem_t sixteen;
// sem_t one;

// volatile int one = 1;

// int* globaldata;
// int* globalhist;

// volatile int sixteen = 1;
// // int one = 0;

// int cnt = 0;        // readers,writers

// pthread_barrier_t barrier;

// void* helper(void* input)
// {
//     // sem_wait(&sixteen);

//     while (sixteen){}

//     // __sync_fetch_and_add(&cnt, 1);

//     int* data = globaldata;
//     int* hist = globalhist;

//     const long thread_id = (long) input;

//     const int STEP = T1N / 32;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T1B]++;               
//     }
    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add(hist+i, temp[i]);
//     }

//     // __sync_fetch_and_sub(&cnt, 1);

//     // if (cnt == 0)
//     // {
//     //     sem_post(&one);
//     // }

//     pthread_barrier_wait(&barrier);
//     one = 0;

//     return NULL;

// }

// void init()
// {
//     pthread_barrier_init(&barrier, NULL, 24);
//     // sem_init(&sixteen, 0, 0);
//     // sem_init(&one, 0, 0);  // initial lock
//     pthread_t thread_id[24];

//     long k = 8;

//     for (int i = 0 ; i < 24 ; i++)
//     {
//         pthread_create(&thread_id[i], NULL, helper, (void*)k);
//         k++;
//     }

//     for (size_t i = 0 ; i < 24 ; i++)
//         pthread_detach(thread_id[i]);
// }

// void* compute_histogram_case1(void* input)
// {
//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     globaldata = (int*)(histArgs->data);
//     globalhist = (int*)(histArgs->hist);

//     const int thread_id = histArgs->id;

//     if (thread_id == 0)
//         sixteen = 0;

//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);

//     const int STEP = T1N / 32;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;



//     if (thread_id == 0)
//     {    
//         int temp[8] = {0};

//         for (int i = start; i < end; i++) {
//              temp[data[i] % T1B]++;               
//         }
    
//         for (int i = 0 ; i < 8 ; i++)
//         {
//              __sync_fetch_and_add(hist+i, temp[i]);
//         }

//         // sem_wait(&one);
//         while (one){}
//     }
//     else
//     {
//         int temp[8] = {0};

//         for (int i = start; i < end; i++) {
//             temp[data[i] % T1B]++;               
//         }
    
//         for (int i = 0 ; i < 8 ; i++)
//         {
//             __sync_fetch_and_add(hist+i, temp[i]);
//         }

//     }

//     return NULL;
// }

/* END */

// pthread_barrier_t barrier;
// pthread_barrier_t barr;
// int suspendmain = 1;
// int suspendhelpers = 1;

// int* globaldata = NULL;
// int* globalhist = NULL;

// void* helper(void* input)
// {
//     while (suspendhelpers){}

//     int* data = globaldata;
//     int* hist = globalhist;
//     const long thread_id = (long) input;

//     const int STEP = T1N / 16;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T1B]++;               
//     }
    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }


//     pthread_barrier_wait(&barrier);

//     if (thread_id == 0) {__sync_bool_compare_and_swap(&suspendmain, 1, 0);}

//     return NULL;
// }

// void init() {

//     pthread_barrier_init(&barrier, NULL, 16);
//     pthread_barrier_init(&barrier, NULL, 8);
//     // sem_init(&mutex, 0, 1);
//     pthread_t thread_id[16];

//     for (long i = 0 ; i < 16 ; i++)
//         pthread_create(&thread_id[i], NULL, helper, (void*)i);

//     for (int i = 0 ; i < 16 ; i++)
//         pthread_detach(thread_id[i]);
// }

// void* compute_histogram_case1(void* input)
// {
//     HistogramArgs* histArgs = (HistogramArgs*) input;

//     const int thread_id = histArgs->id;

//     if (thread_id == 0)
//     {
//         globaldata = histArgs->data;
//         globalhist = histArgs->hist;


//         suspendhelpers = 0;
//         while(suspendmain){}
//     }

//     pthread_barrier_wait(&barr);

//     return NULL;
// }

/* attempts at additional multi threading */

// void* helper (void* input)
// {
//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);

//     const int thread_id = histArgs->id;

//     const int STEP = T1N / 16;
//     const int start = (thread_id+8) * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T1B]++;                 
//     }

    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }
    

//     return NULL;
// }

// void* compute_histogram_case1(void* input) {

//     pthread_t extra;
//     pthread_create(&extra, NULL, helper, input);

//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);

//     const int thread_id = histArgs->id;

//     const int STEP = T1N / 16;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T1B]++;                 
//     }

    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }
    
//     pthread_join(extra, NULL);

//     return NULL;
// } 

// void* helper(void* input)
// {
//     HistogramArgs* histArgs = (HistogramArgs*)input;
//     int* data = (int*)(histArgs->data);
//     int* hist = (int*)(histArgs->hist);

//     const int thread_id = histArgs->id;

//     const int STEP = T1N / 32;
//     const int start = thread_id * STEP;
//     const int end = start + STEP;

//     int temp[8] = {0};

//     for (int i = start; i < end; i++) {
//         temp[data[i] % T1B]++;
//     }

    
//     for (int i = 0 ; i < 8 ; i++)
//     {
//         __sync_fetch_and_add( hist+i, temp[i]);
//     }

//     return NULL;
// }

// void* compute_histogram_case1(void* input)
// {

//     HistogramArgs* histArgs = (HistogramArgs*) input;

//     pthread_t thread_id[4];
//     HistogramArgs histArgsArray[4];
//     void* argsPtrArray[4] = {0};

//     for (int i = 0; i < 4; i++) {
//         histArgsArray[i].data = histArgs->data;
//         histArgsArray[i].hist = histArgs->hist;
//         histArgsArray[i].id = (histArgs->id) + (i << 3);        // id = 0 - 23;

//         argsPtrArray[i] = &histArgsArray[i];
//     }

//     int i;
//     for (i = 0; i < 4; i++) {
//         pthread_create(&thread_id[i], NULL, helper, argsPtrArray[i]);
//     }

//     for (i = 0; i < 4; i++) {
//         pthread_join(thread_id[i], NULL);
//     }

//     return NULL;
// }



/* *** ORIGINAL TEST CASE 1 CODE **** */
//     // HistogramArgs* histArgs = (HistogramArgs*)input;
//     // int* data = (int*)(histArgs->data);
//     // int* hist = (int*)(histArgs->hist);
//     // const int thread_id = histArgs->id;

//     // if (thread_id == 0) {
        
//     //     for (int j = 0; j < T1N; j++) {
//     //         hist[data[j] % T1B]++;
//     //     }
//     // }
//     // return NULL;







/*  ============= 

Add corresponding locks/semaphores and any other global variables here
Sample declarations:
sem_t mutex; //don't forget to initialize in main
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_barrier_t barrier; //don't forget to initialize in init
    pthread_mutex_t locks[BUCKET_SIZE]; //don't forget to initialize in init

=============  */

 /*  ============= 
    add initialization code for locks, or other global variables defined earlier

    the init() function will be called once at the beginning of each trial,
    as shown in the pseudocode below

    for (int i = 0; i < num_trials; i++) {
        init();
        test_results.append(run_tests(all_cases_or_a_particular_case));
    }
    =============  */



    /*  =============
    this is the thread worker function that will be called for test case 1
    you can assume that there are T1N data points T1B histogram buckets

    This function will be called for each of the NTHREADS (8) worker threads

    The data array pointer (int* data) and the histogram array pointer (int* hist)
    will be the same for each thread. 

    thread_id would be different for each thread: each of the 8 worker 
    threads will get a different thread_id ranging [0, 7] (inclusive on both sides)

    The function given below is effectively only "single-threaded", because only one
    thread (with thread_id 0) does all the work, while all the other threads returns
    immediately. As a result, the performance is not that good.

    You should modify this function to effectively use the pool of the 8 worker 
    threads to compute the histogram faster.
    =============  */

/*  =============

This commented out function tries to divide up the data into
blocks, one for each thread. However, there is a race! (where?)

Try it out!

void* compute_histogram_case1_RACE_CONDITION(void* input) {
    HistogramArgs* histArgs = (HistogramArgs*)input;
    int* data = (int*)(histArgs->data);
    int* hist = (int*)(histArgs->hist);
    const int thread_id = histArgs->id;

    const int STEP = T1N / NTHREADS;
    const int start = thread_id * STEP;
    const int end = start + STEP;

    for (int i = start; i < end; i++) {
        hist[data[i] % T1B]++;
    }

    return NULL;
}
 =============
    this function will be called for test case 2
    you can assume that there are T2N data points T2B histogram buckets

    This function will be called for each of the NTHREADS (i.e., 8) worker threads

    The data array pointer (int* data) and the histogram array pointer (int* hist)
    will be the same for each thread. 

    thread_id would be different for each thread: each of the 8 worker 
    threads will get a different thread_id ranging [0, 7] (inclusive on both sides)

    The function given below is effectively only "single-threaded", because only one
    thread (with thread_id 0) does all the work, while all the other threads returns
    immediately. As a result, the performance is not that good.

    You should modify this function to effectively use the pool of the 8 worker 
    threads to compute the histogram faster.
    =============  */