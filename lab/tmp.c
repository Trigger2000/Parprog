#include <mpi.h>
#include <stdio.h>

int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int token = 5;
        MPI_Ssend(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        int token = 0;
        MPI_Ssend(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("received message %d\n", token);
    }
    
    MPI_Finalize();
}