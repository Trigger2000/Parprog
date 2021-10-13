/* 
    Здесь представлено "честное" решение задачи,
    альтернативное можно найти в not_honest.cpp
    Доп условие выполнено 
*/

#include <iostream>
#include <stdlib.h>

#include <omp.h>

/*
    Алгоритм:
    1) Пропускаем вычисление первого элемента "подмассива" в каждом потоке,
    сохраняя изначальные значения левого и правого элементов относительно первого элемента
    во вспомогательную структуру neighbours_of_first. 
    2) Вычисляем все значения со второго элемента до последнего в каждом "подмассиве"
    3) Синхронизируем все потоки
    4) Вычисляем значение первого элемента "подмассива" в каждом потоке
*/

struct neighbours_of_first
{
    int left_neighbour = 0;
    int right_neighbour = 0;
};

int main()
{
    unsigned long long array[100000];

    // инициализируем массив
    #pragma omp parallel for shared(array)
    for (int i = 0; i < 100000; ++i)
    {
        array[i] = i;
    }

    int max_threads = omp_get_max_threads();
    neighbours_of_first *helper = new neighbours_of_first[max_threads];

    #pragma omp parallel shared(array, helper)
    {
        unsigned long long previous_for_current = 0, previous_for_next = 0;
        unsigned long long first_element = 0;
        int thread_num = omp_get_thread_num();
        #pragma omp for
        for (unsigned long long i = 1; i < 99999; ++i)
        {
            // пропуск первого элемента
            if (first_element == 0)
            {
                helper[thread_num].left_neighbour = array[i - 1];
                // строго говоря, тут должен быть барьер или другое средство синхронизации
                helper[thread_num].right_neighbour = array[i + 1];
                first_element = i;
                previous_for_next = array[i];
                ++i;
            }
            previous_for_current = previous_for_next;
            previous_for_next = array[i];
            array[i] = previous_for_current * array[i] * array[i + 1] / 3;
        }

        #pragma omp barrier
        array[first_element] = helper[thread_num].left_neighbour * array[first_element] * helper[thread_num].right_neighbour / 3;
    }

    delete[] helper;
    return 0;
}