
#include <mpi.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

#define xTag 1
#define yTag 2
#define sumTag 3
#define multTag 4
#define rankTag 5
using namespace std;
#include <iostream>

int main2() {
	int size, rank, count;
	MPI_Status status;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (size < 2) {
		printf("Not enough processes! Need > 1");
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_TOPOLOGY);
		return 1;
	}

	if (rank == 0) {
		int x[] = { 22, 33, 44, 55, 66,77,8,9,90 };
		int y[] = { 122, 133, 144, 155, 166,77,8,9,90};
		//printf("sizeofx[0]%d\n", sizeof(x[0]));
		//printf("sizeofx%d\n", sizeof(x));
		int vect_size = sizeof(x) / sizeof(x[0]);
		int block_size = ceil(vect_size / (double)(size - 1));
		int need_procs = ceil(vect_size / (double)block_size);
		int max_rank[] = { need_procs };

		for (int i = 1; i < size; ++i)
			MPI_Send(max_rank, 1, MPI_INT, i, rankTag, MPI_COMM_WORLD);

		int* res_sum = new int[vect_size];
		int* res_mult = new int[vect_size];

		for (int i = 0, proc_num = 1; i < vect_size; ++proc_num) {
			if (block_size > vect_size - i)
				block_size = vect_size - i;

			int* x_block = new int[block_size];
			int* y_block = new int[block_size];
			for (int j = 0; j < block_size; ++j, ++i) {
				x_block[j] = x[i];
				y_block[j] = y[i];
			}
			MPI_Send(x_block, block_size, MPI_INT, proc_num, xTag, MPI_COMM_WORLD);
			printf("Proc 0: block_x -> proc. %d\n", proc_num);
			MPI_Send(y_block, block_size, MPI_INT, proc_num, yTag, MPI_COMM_WORLD);
			printf("Proc 0: block_y -> proc. %d\n", proc_num);
			delete[] x_block;
			delete[] y_block;
		}

		block_size = ceil(vect_size / (double)(size - 1));
		for (int i = 1; i <= need_procs; ++i) {
			// get block with sum from proc. i
			MPI_Probe(i, sumTag, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &count);
			MPI_Recv(res_sum + block_size * (i - 1), count, MPI_INT, i, sumTag, MPI_COMM_WORLD, &status);
			printf("Proc 0: sum_block recvd from proc. %d\n", i);

			// get block with multiplication result from proc. i
			MPI_Probe(i, multTag, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &count);
			MPI_Recv(res_mult + block_size * (i - 1), count, MPI_INT, i, multTag, MPI_COMM_WORLD, &status);
			printf("Proc 0: multiply_block recvd from proc. %d\n", i);
		}
		printf("%d", vect_size);
		printf("\nVector x:\n\n");
		for (int i = 0; i < vect_size; ++i) {
			printf("%d ", x[i]);
		}
		printf("\nVector y:\n\n");
		for (int i = 0; i < vect_size; ++i) {
			printf("%d ", y[i]);
		}
		printf("\nVector x+y:\n\n");
		for (int i = 0; i < vect_size; ++i) {
			printf("%d ", res_sum[i]);
		}
		printf("\nVector x*y:\n\n");
		for (int i = 0; i < vect_size; ++i) {
			printf("%d ", res_mult[i]);
		}
		delete[] res_sum;
		delete[] res_mult;
	}
	else {
		int max_rank[1];
		MPI_Recv(max_rank, 1, MPI_INT, 0, rankTag, MPI_COMM_WORLD, &status);
		if (rank <= max_rank[0]) {

			
			MPI_Probe(0, xTag, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &count);

			// catch block of 'x' elems
			int* x_arr = new int[count];
			MPI_Recv(x_arr, count, MPI_INT, 0, xTag, MPI_COMM_WORLD, &status);
			printf("Proc %d: block_x recvd from 0 proc 0\n", rank);

			// catch block of 'y' elems
			int* y_arr = new int[count];
			MPI_Recv(y_arr, count, MPI_INT, 0, yTag, MPI_COMM_WORLD, &status);
			printf("Proc %d: block_y recvd from 0 proc\n", rank);

			int* res_sum = new int[count];
			int* res_mult = new int[count];
			for (int i = 0; i < count; ++i) {
				res_sum[i] = x_arr[i] + y_arr[i];
				res_mult[i] = x_arr[i] * y_arr[i];
			}

			// send back computed blocks
			MPI_Send(res_sum, count, MPI_INT, 0, sumTag, MPI_COMM_WORLD);
			printf("Process %d: sum-block -> 0\n", rank);
			MPI_Send(res_mult, count, MPI_INT, 0, multTag, MPI_COMM_WORLD);
			printf("Process %d: mult-block -> 0\n", rank);

			delete[] x_arr;
			delete[] y_arr;
			delete[] res_sum;
			delete[] res_mult;
		}
	}

	MPI_Finalize();
	return 0;
}