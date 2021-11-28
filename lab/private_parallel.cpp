#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 100
#define JSIZE 100

// double a[ISIZE][JSIZE], b[ISIZE][JSIZE];
double *a;
double *b;

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int com_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &com_size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Com size is %d\n", com_size);

    // кол-во элементов, обрабатываемых одним процессом
    int row_count = (ISIZE - 4) / com_size;
    int data_count = row_count * JSIZE;
    a = (double*)calloc(data_count, sizeof(double));
    b = (double*)calloc(data_count, sizeof(double));
    // double *b = (double*)calloc(data_count, sizeof(double));

    int i, j;
    // как и в случае с "общей задачей", распараллеливание удобнее всего осуществлять по внешнему индексу
    for (i = 0; i < row_count; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i * JSIZE + j] = 10 * (row_count * rank + i + 4) + j;
        }
    }

    for (i = 0; i < row_count; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i * JSIZE + j] = sin(0.00001 * a[i * JSIZE + j]);
        }
    }

    for (i = 0; i < row_count; i++)
    {
        for (j = 1; j < JSIZE; j++)
        {
            b[i * JSIZE + j] = a[i * JSIZE + j - 1] * 1.5;
        }
    }

    double *data = (double*)calloc(ISIZE * JSIZE, sizeof(double));
    MPI_Gather(b, data_count, MPI_DOUBLE, data, data_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // отдельно обработаем остаток, если он есть
    if (row_count * com_size != ISIZE - 4 && rank == 0)
    {
        int remainder = (ISIZE - 4) % com_size;
        double *a_helper = (double*)calloc(remainder * JSIZE, sizeof(double));
        int i, j;
        for (i = 0; i < remainder; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                a_helper[i * JSIZE + j] = 10 * (row_count * com_size + i + 4) + j;
            }
        }

        for (i = 0; i < remainder; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                a_helper[i * JSIZE + j] = sin(0.00001 * a_helper[i * JSIZE + j]);
            }
        }

        for (i = 0; i < remainder; i++)
        {
            for (j = 1; j < JSIZE; j++)
            {
                data[(row_count * com_size + i) * JSIZE + j] = a_helper[i * JSIZE + j - 1] * 1.5;
            }
        }
        free(a_helper);
    }

    if (rank == 0)
    {
        FILE *ff = fopen("result.txt","w");
        for (i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                fprintf(ff,"%f ",data[i * JSIZE + j]);
            }
            fprintf(ff,"\n");
        }
        fclose(ff);
    }

    free(data);
    free(a);
    free(b);
    MPI_Finalize();
}