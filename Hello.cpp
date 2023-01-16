#include <iostream>
#include <mpi.h>

int main100() {

    int rank, size;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello World from %d proc, Max. num of proc %i\n", rank, size);
    MPI_Finalize();

    return 0;
}