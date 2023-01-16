#include "mpi.h"
#include <iostream>
#include <limits>
#include <array>
#include <memory>
#include "stdlib.h"
#include "math.h"

#include "randvector.h"

using namespace std;


#define MAS_SIZE 10


int main1233(int argc, char* argv[]) {

    srand(time(0));
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1)
    {
        array<int, MAS_SIZE> a;
        rand_vec(a.data(), MAS_SIZE);
        for (size_t i = 0; i < size; i++) {
            if (i == 1) {
                continue;
            }
            const size_t procSendTo = i;
            MPI_Send((void*)a.data(), (int)a.size(), MPI_INT, (int)procSendTo, MAS_SIZE, MPI_COMM_WORLD);
        }
    }

    if (rank != 1) {
        MPI_Status status;
        int count;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);

        auto buff = make_unique<int[]>(count);
        MPI_Recv((void*)buff.get(), (int)count, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

        for (size_t i = 0; i < count; i++) {
            printf("Element %u\t : id %lu\t on process %u from %u process, %s\n",
                buff.get()[i], i, rank, status.MPI_SOURCE, status.MPI_ERROR ? "no errors" : "error!");
        }
    }

    MPI_Finalize();
    return 0;
}