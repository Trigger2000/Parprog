#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define FACTORIAL_LIMIT 200

long double calculate(int size, int rank, long double* factorial);

int main(int argc, char** argv)
{
    int i = 0;
    long double* factorial = (long double*)calloc(FACTORIAL_LIMIT, sizeof(long double));
    factorial[0] = 1;
    for (i = 1; i < FACTORIAL_LIMIT; ++i)
    {
        factorial[i] = factorial[i - 1] * i;
    }

    int size = 0, rank = 0;
    long double* result;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        int tmp = 1;
        long double res = 0.0;
        result = (long double*)calloc(size + 2, sizeof(long double));
        for (int i = 1; i < size; ++i)
        {
            MPI_Send(&tmp, 1, MPI_INT, i, i, MPI_COMM_WORLD);
        }

        res = calculate(size, rank, factorial);

        for (int i = 1; i < size; ++i)
        {
            long double rec = 0.0;
            MPI_Recv(&rec, 1, MPI_LONG_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            res += rec;
        }

        printf("e is %.62LF\n", res);

        free(factorial);
        free(result);
    }
    else
    {
        int tmp = 0;
        MPI_Recv(&tmp, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        long double part_res = calculate(size, rank, factorial);
        MPI_Send(&part_res, 1, MPI_LONG_DOUBLE, 0, rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

long double calculate(int size, int rank, long double* factorial)
{
    long double part_e = 0.0;
    
    int tmp = FACTORIAL_LIMIT;
    int counter = 0;
    while (tmp % size != 0)
    {
        tmp--;
        counter++;
    }
    int step = tmp / size;
    
    int begin = rank*step;
    int end = 0;
    if (rank == size - 1)
    {
        end = begin + step + counter;
    }
    else
    {
        end = begin + step;
    }

    const long double one = 1.0;
    int i = begin;
    for (i; i < end; ++i)
    {
        part_e += one / factorial[i];
    }
    return part_e;
}