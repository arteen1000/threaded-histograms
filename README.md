# threadlab
Scored best in class of 250 competitors. Link to [competition](https://polyarch.github.io/cs33/labs/lab5/). Speedup of 8.6 in normal (large data, few buckets) and a speedup of 4.2 in pathological case (large number of buckets, less data) after synchronizing usage of 8 threads using various algorithms and methods. Used `pthreads.h` library in C. Implementation in `histo.c`.

Here are the relevant performance characteristics:
```
T#  Kernel |   Elements    Buckets | Time(ms)    CPE  Speedup
 1   Histo |  100000000          8 |     23.5  0.470      8.6
 2   Histo |   25000000   16000000 |     94.1  7.529      4.3
Geomean Speedup: 6.08
```

Find related kernel optimization project [here](https://github.com/arteen1000/performant-kernels).
