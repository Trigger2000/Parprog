/*
    Решил задачу с доп условием (дополнительное выделение массива отсутствует)
    Ради упрощения ввода использован такой формат:
    первым числом вводится количество элементов, а дальше сами элементы

    В ex.dat файле содержится пример с 5000000 чисел, на таком количестве данных заметно ускорение
*/


#include <iostream>
#include <fstream>
#include <string>

#include <omp.h>

int32_t* scan_array(int *size);
void my_qsort(int32_t *array, int lower_bound, int upper_bound);
int partition(int32_t *array, int lower_bound, int upper_bound);

int main()
{
    int size = 0;
    // вводим элемент с клавиатуры или через файл
    int32_t *array = scan_array(&size);
    #pragma omp parallel
        #pragma omp single
        {
            // сортировка массива
            my_qsort(array, 0, size - 1);
        }

    for (int i = 0; i < size; ++i)
    {
        std::cout << array[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}

int32_t* scan_array(int *size)
{
    std::cout << "type\n0 - read array from a keyboard\n1 - read array from a file\n";
    int input_type = -1;
    std::cin >> input_type;
    if (input_type != 0 && input_type != 1)
    {
        std::cout << "Incorrect input type, exiting\n";
        exit(EXIT_FAILURE);
    }

    int32_t* array = nullptr;
    if (input_type == 0)
    {
        std::cout << "Type number of array's elements\n";
        std::cin >> *size;
        array = new int32_t[*size];
        std::cout << "type array\n";
        for (int i = 0; i < *size; ++i)
        {
            std::cin >> array[i];
        }
    }

    if (input_type == 1)
    {
        std::cout << "Type filename\n";
        std::string filename;
        std::cin >> filename;
        std::ifstream infs(filename);

        if (!infs)
        {
            std::cout << "invalid filename, exiting\n";
            exit(EXIT_FAILURE);
        }

        infs >> *size;
        array = new int32_t[*size];
        for (int i = 0; i < *size; ++i)
        {
            infs >> array[i];
        }
    }

    return array;
}

void my_qsort(int32_t *array, int lower_bound, int upper_bound)
{
    if (lower_bound < upper_bound)
    {
        int q = partition(array, lower_bound, upper_bound);
        // распараллеливается сортировка подмассивов
        #pragma omp task
            my_qsort(array, lower_bound, q);
        #pragma omp task
            my_qsort(array, q + 1, upper_bound);
    }
}

int partition(int32_t *array, int lower_bound, int upper_bound)
{
    int32_t mid = array[static_cast<int>((lower_bound + upper_bound) / 2)];
    while (lower_bound <= upper_bound)
    {
        while (array[lower_bound] < mid)
            ++lower_bound;
        while (array[upper_bound] > mid)
            --upper_bound;
        if (lower_bound >= upper_bound)
            break;
        
        int tmp = array[upper_bound];
        array[upper_bound] = array[lower_bound];
        array[lower_bound] = tmp;

        ++lower_bound;
        --upper_bound;
    }
    return upper_bound;
}