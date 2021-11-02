/******************************************************************************
* ЗАДАНИЕ: bugged2.c
* ОПИСАНИЕ:
*   Еще одна программа на OpenMP с багом. 
******************************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    // лучше инициализировать итоговую сумму тут нулем, чем в параллельной секции
    float total = 0.0;

    // переменные i и tid необходимо сделать приватными
    #pragma omp parallel private(tid, i) 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        // пусть для начала выведется кол-во потоков, так будет логичнее
        #pragma omp barrier
        printf("Thread %d is starting...\n", tid);

        #pragma omp barrier

        // необходимо безопасное вычисление суммы с помощью reduction
        // также для увеличения точности вычисления лучше заменить schedule(dynamic)
        // на schedule(static), но в целом и так сойдет
        #pragma omp for schedule(dynamic, 10) reduction(+:total)
        for (i = 0; i < 1000000; i++) 
            total = total + i*1.0;

        
        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
}