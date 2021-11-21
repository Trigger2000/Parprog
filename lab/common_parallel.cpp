#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// #include <chrono>

#define ISIZE 10000
#define JSIZE 10000

int main(int argc, char **argv)
{
    // std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

    MPI_Init(NULL, NULL);

    int com_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Com size is %d\n", com_size);

    // кол-во элементов, обрабатываемых одним процессом
    int data_count = ISIZE * JSIZE / com_size;
    double *proccess_buf = (double*)calloc(data_count, sizeof(double));

    // std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
    // std::cout << "Time difference of setting = " << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1).count() << "[ms]" << std::endl;

    int i, j;
    for (i = 0; i < ISIZE / com_size; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            // чтобы результат был одинаковый, надо пересчитать множитель
            proccess_buf[ISIZE * i + j] = 10 * (ISIZE / com_size * rank + i) + j;
        }
    }

    for (i = 0; i < ISIZE / com_size; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            proccess_buf[ISIZE * i + j] = sin(0.00001 * proccess_buf[ISIZE * i + j]);
        }
    }

    // std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
    double *data = (double*)calloc(ISIZE * JSIZE, sizeof(double));
    MPI_Gather(proccess_buf, data_count, MPI_DOUBLE, data, data_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    // std::cout << "Time difference of gathering = " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2).count() << "[ms]" << std::endl;
    free(proccess_buf);

    if (rank == 0)
    {
        FILE *ff = fopen("result.txt","w");
        for(i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                fprintf(ff, "%f ", data[i * ISIZE + j]);
            }

            fprintf(ff, "\n");
        }
        fclose(ff);
    }

    free(data);
    MPI_Finalize();
    return 0;
}