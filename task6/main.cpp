/*
    Решил задачу с доп условием:
    Использовано решето Эратосфена для поиска простых чисел
*/

#include <iostream>
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

    if (N == 1)
    {
        std::cout << 0 << '\n';
        return 0;
    }

    int *array = new int[N];
    int res = 0;

    // вычисление с помощью решета Эратосфена
    for (int i = 2; i < N; ++i)
    {
        if (array[i] == 0)
        {
            res++;
            // вроде бы оптимальнее всего распараллеливать внутренний цикл
            #pragma parallel for shared(array, N, i)
            for (int j = i*i; j < N; j += i)
            {
                array[j] = 1;
            }
        }
    }

    delete[] array;
    std::cout << res << '\n';

    return 0;
}