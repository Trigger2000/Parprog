/*
    Решил задачу с доп условием(другой файл)
    Здесь вычисление экспоненты
*/

#include <iostream>
#include <cmath>

#include <stdlib.h>

#include <omp.h>

#define X 1

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

    // массив степеней и факториалов лучше вычислить предварительно
    double *array_x = (double*)calloc(N, sizeof(double));
    double *array_factorial = (double*)calloc(N, sizeof(double));
    array_x[0] = 1;
    array_factorial[0] = 1;
    for (int i = 1; i < N; ++i)
    {
        array_x[i] = array_x[i - 1] * X;
        array_factorial[i] = array_factorial[i - 1] * i;
    }

    #pragma omp parallel shared(N, sum, array_x, array_factorial)
    {
        long double sum_local = 0.0;
        #pragma omp for
        // высиление ряда
        for (int i = 0; i < N; ++i)
        {
            sum_local += array_x[i] / array_factorial[i];
        }
        
        // запись результата в общую переменную
        omp_set_lock(&lock);
        sum += sum_local;
        omp_unset_lock(&lock);
    }

    std::cout << sum << '\n';

    return 0;
}