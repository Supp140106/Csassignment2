#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define N 1000

double A[N][N], B[N][N], C[N][N], C2[N][N];

double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

void init()
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
        {
            A[i][j]=1;
            B[i][j]=1;
            C[i][j]=0;
            C2[i][j]=0;
        }
}

void sequential()
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            for(int k=0;k<N;k++)
                C[i][j]+=A[i][k]*B[k][j];
}

void parallel(int PROCESSES)
{
    int rows = N/PROCESSES;

    for(int p=0;p<PROCESSES;p++)
    {
        pid_t pid = fork();

        if(pid==0)
        {
            int start = p*rows;
            int end = (p==PROCESSES-1)?N:start+rows;

            for(int i=start;i<end;i++)
                for(int j=0;j<N;j++)
                    for(int k=0;k<N;k++)
                        C2[i][j]+=A[i][k]*B[k][j];

            exit(0);
        }
    }

    for(int p=0;p<PROCESSES;p++)
        wait(NULL);
}

int main()
{
    int PROCESSES = sysconf(_SC_NPROCESSORS_ONLN);

    printf("Available Cores: %d\n", PROCESSES);

    init();

    double t1 = get_time();
    sequential();
    double t2 = get_time();
    printf("Sequential Time: %f seconds\n", t2-t1);

    double t3 = get_time();
    parallel(PROCESSES);
    double t4 = get_time();
    printf("Parallel Time: %f seconds\n", t4-t3);

    return 0;
}