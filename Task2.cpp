#include "mpi.h"
#include <iostream>


int main123(int argc, char* argv[]) {

    int rank, size;
    const int n = 10;
    int a[n];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status st;
    if (!rank)
    {
        srand(time(0));
        for (int i = 0; i < n; i++)
            a[i] = rand() % 10;
        MPI_Send(a, n, MPI_INT, 1, 10, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(a, n, MPI_INT, 0, 10, MPI_COMM_WORLD, &st);
    }
    if (rank == 1)
    {
        for (int i = 0; i < n; i++)
            printf("a[%d]=%d proc: %d\n", i, a[i], rank);
    }
    MPI_Finalize();
    return 0;

}