#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include <iostream>

#define ISIZE 1000
#define JSIZE 1000

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int com_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::cout << com_size << "\n";
    // для передачи по mpi лучше аллоцировать двумерный массив как одномерный
    // и затем для работы с ним переводить к double** (строчки 27 и 53)
    double *data = (double*)calloc(ISIZE * JSIZE, sizeof(double));

    int sending_count = ISIZE * JSIZE / com_size;
    double *rbuf = (double*)calloc(sending_count, sizeof(double));
    MPI_Scatter(data, sending_count, MPI_DOUBLE, rbuf, sending_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double **a = (double**)calloc(ISIZE, sizeof(double*));
    for (int i = 0; i < ISIZE / com_size; ++i)
        a[i] = &(rbuf[JSIZE * i]);

    int i, j;
    for (i = 0; i < ISIZE / com_size; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            // чтобы результат был одинаковый, надо пересчитать множитель
            a[i][j] = 10 * (ISIZE / com_size * rank + i) + j;
        }
    }

    for (i = 0; i < ISIZE / com_size; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = sin(0.00001 * a[i][j]);
        }
    }

    MPI_Gather(rbuf, sending_count, MPI_DOUBLE, data, sending_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double **a = (double**)calloc(ISIZE, sizeof(double*));
        for (int i = 0; i < ISIZE; ++i)
            a[i] = &(data[JSIZE * i]);

        FILE *ff = fopen("result.txt","w");
        for(i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                fprintf(ff, "%f ", a[i][j]);
            }

            fprintf(ff, "\n");
        }
        fclose(ff);
    }

    MPI_Finalize();
    return 0;
}