/*
    Решил задачу с доп условием:
    вычисляется ряд Фурье функции y = x^2, заданной на отрезке [-Pi, Pi],
    2*Pi периодически продолженной на всю числовую ось, в точке x = 2
*/

#include <iostream>
#include <cmath>

#include <stdlib.h>

#include <omp.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("give me N\nUsage: ./compile_and_run.sh [N]\n");
        return 1;
    }
    // считывание N из аргументов запуска
    int N = strtol(argv[1], NULL, 10);

    long double sum = 0.0;

    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel shared(N, sum)
    {
        long double sum_local = 0.0;
        #pragma omp for
        // высиление ряда
        for (int i = 1; i < N; ++i)
        {
            if (i % 2 == 0)
            {
                sum_local += 4.0 / (i * i) * std::cos(i * 2.0);
            }
            else
            {
                sum_local -= 4.0 / (i * i) * std::cos(i * 2.0);
            }
        }
        
        // запись результата в общую переменную
        omp_set_lock(&lock);
        sum += sum_local;
        omp_unset_lock(&lock);
    }

    sum += 3.14 * 3.14 / 3.0; // прибавление константы а_0

    std::cout << sum << '\n';

    return 0;
}