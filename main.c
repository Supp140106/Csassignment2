#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>


double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}


double *alloc_matrix(int n) {
    return (double *)malloc(sizeof(double) * n * n);
}

void fill_matrix(double *M, int n) {
    for (int i = 0; i < n * n; i++)
        M[i] = random() % 10;
}

