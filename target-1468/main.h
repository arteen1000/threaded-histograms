#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <sys/time.h>

static __inline__ uint64_t read_usec(void) {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (((uint64_t)tv.tv_sec) * 1000000 + ((uint64_t)tv.tv_usec));
}

#endif
