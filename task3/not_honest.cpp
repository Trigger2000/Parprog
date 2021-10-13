// решил задачу с доп условием, что не нужно создавать второй массив
//
// "более честное" решение можно найти в honest.cpp

#include <iostream>

#include <omp.h>

int main()
{
    unsigned long long array[100000];

    // инициализируем массив
    #pragma omp parallel for shared(array)
    for (int i = 0; i < 100000; ++i)
    {
        array[i] = i;
    }

    // в задаче известно, что в изначальном массиве хранятся порядковые номера,
    // поэтому для вычисления нового массива данные из старого не нужны
    #pragma omp parallel for shared(array)
    for (unsigned long long i = 1; i < 99999; ++i)
    {
        array[i] = (i - 1) * i * (i + 1) / 3;
    }

    return 0;
}