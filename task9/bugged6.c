/******************************************************************************
* ЗАДАНИЕ: bugged6.c
* ОПИСАНИЕ:
*   Множественные ошибки компиляции
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

int main (int argc, char *argv[])
{
    int i;
    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;

    // Проще всего сделать inlining функции
    #pragma omp parallel shared(sum, a, b)
    {
        // лучше переименовать индекс
        int j, tid;

        tid = omp_get_thread_num();
        #pragma omp for reduction(+:sum)
        for (j = 0; j < VECLEN; j++)
        {
            sum = sum + (a[j] * b[j]);
            printf("  tid= %d i=%d\n", tid, j);
        }
    }

    printf("Sum = %f\n",sum);
}


