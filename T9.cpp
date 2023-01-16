#include <cstdio>
#include <mpi.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

constexpr int GENERAL_PROCESS = 0;
constexpr int ARRAY_SIZE = 12;

struct array_elem_pair_t {
	int elem;
	int i;
};

void print_array(array_elem_pair_t* a, int size) {
	for (auto i = 0; i < size; i++) {
		printf("%d ", a[i].elem);
	}
	printf("\n");
}

void init_array(array_elem_pair_t* a, int size) {
	for (auto i = 0; i < size; i++) {
		// [-100..100]
		a[i].elem = rand() % 2001 - 1000;
		a[i].i = i;
	}
}

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);

	srand(time(0));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 4) {
		if (rank == 0) {
			printf("Not enough processors! Need exactly 4\n");
		}

		MPI_Finalize();
		return 1;
	}

	array_elem_pair_t local_min_elem_pair;
	array_elem_pair_t* array=nullptr;
	array_elem_pair_t* buf = new array_elem_pair_t[ARRAY_SIZE / size];

	if (rank == 0) {
		// init array on master
		array = new array_elem_pair_t[ARRAY_SIZE];
		init_array(array, ARRAY_SIZE);
		print_array(array, ARRAY_SIZE);
	}
	MPI_Scatter(array, ARRAY_SIZE / size, MPI_2INT, buf, ARRAY_SIZE / size, MPI_2INT, 0, MPI_COMM_WORLD);


	local_min_elem_pair = buf[0];
	for (int i = 1; i < ARRAY_SIZE / size; ++i) {
		if (buf[i].elem < local_min_elem_pair.elem) {
			local_min_elem_pair = buf[i];
		}
	}
	printf("Local min element: %d at global rank %d\n", local_min_elem_pair.elem, rank); 
	/*local_min_elem_pair = *std::min_element(buf, buf + ARRAY_SIZE / size, [](auto const lhs, auto const rhs) {
		return lhs.elem < rhs.elem;
		});
	printf("Local min element: %d at global rank %d\n", local_min_elem_pair.elem, rank);*/

	array_elem_pair_t min_result;
	MPI_Reduce(&local_min_elem_pair, &min_result, size, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Min element: %d at index %d\n", min_result.elem, min_result.i);
		delete[] array;
	}

	delete[]buf;
	MPI_Finalize();
	return 0;
}