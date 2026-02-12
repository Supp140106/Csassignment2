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
void multiply_sequential(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            C[i*n + j] = 0;
            for (int k = 0; k < n; k++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];
        }
}


void compute_rows(double *A, double *B, int n, int start, int end) {
double *C_part = (double*)malloc(sizeof(double) * (end - start)*n);
for (int i = start; i < end; i++) 
    for (int j = 0; j < n; j++) { 
        C_part[(i-start)*n + j] = 0; 
        for (int k = 0; k < n; k++)
C_part[(i-start)*n + j] += A[i*n + k] * B[k*n + j];
    }
free(C_part);
}


void multiply_parallel(double *A, double *B, int n, int m) {
    int rows_per_child = n / m;
    for (int p = 0; p < m; p++) {
        pid_t pid = fork();
        if (pid == 0) {
            int start = p * rows_per_child;
            int end = (p == m - 1) ? n : start + rows_per_child;
            compute_rows(A, B, n, start, end);
            exit(0);
        }
    }
