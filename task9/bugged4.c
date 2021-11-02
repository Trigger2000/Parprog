/******************************************************************************
* ЗАДАНИЕ: bugged4.c
* ОПИСАНИЕ:
*   Очень простая программа с segmentation fault.
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;
    // нельзя так много памяти на стеке выделять
    // тут stack overflow, выделю в куче
    double **a = (double**)calloc(N, sizeof(double*));
    for (int k = 0; k < N; ++k)
    {
        a[k] = (double*)calloc(N, sizeof(double));
    }

    // раз выделили в куче массив, тогда его надо сделать доступным всем
    #pragma omp parallel shared(nthreads, a) private(i, j, tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        // хотим, чтобы сначала вывелось число потоков
        #pragma omp barrier
        printf("Thread %d starting...\n", tid);

        // этот участок надо сделать критической секцией
        #pragma omp critical
        {
            for (i = 0; i < N; i++)
                for (j = 0; j < N; j++)
                    a[i][j] = tid + i + j;

            printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);
        }
    }

    // удалим то, что насоздавали
    for (int i = 0; i < N; ++i)
    {
        free(a[i]);
    }
    free(a);
}
