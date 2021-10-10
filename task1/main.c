// решил задачу с доп условием про обратный вывод

#include <stdio.h>
#include <unistd.h>

#include <omp.h>

int main()
{
    // для синхронизации и вывода id потоками в обратном порядке используется
    // переменная last_printed_id, которая хранит номер последнего потока, сделавшего вывод
    int last_printed_id = omp_get_max_threads();
    #pragma omp parallel
    {
        int cur_id = omp_get_thread_num();
        // ждем, пока поток с номером id+1 не сделает вывод
        while (last_printed_id != cur_id + 1);
        printf("Hello world from thread %d\n", cur_id);
        last_printed_id = cur_id;
    }
    return 0;
}