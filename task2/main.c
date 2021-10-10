// Решил задачу с доп условиями:
// 1) Верхняя граница суммирования может быть введена в аргументах запуска скрипта
// 2) Использованы schedule и reduction

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("give me N\nUsage: ./compile_and_run.sh [N]\n");
        return 1;
    }
    // считывание N из аргументов запуска
    int N = strtol(argv[1], NULL, 10);

    int sum_i = 0;
    // вычисление
    #pragma omp parallel for schedule(dynamic, 100) reduction(+:sum_i) 
        for (int i = 0; i <= N; ++i)
        {
            sum_i += i;
        }

    printf("%d\n", sum_i);

    return 0;
}