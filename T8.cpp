#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector> 


using namespace std;

//------------
// CONSTANTS
//------------

const int N = 10;

// tags
const int DATA = 0;
const int CONTROL = 1;

// control codes
const int END = 0;
const int CONTINUE = 1;
const int EXIST = 2;
const int NOTEXIST = 3;


//------------
// UTILS
//------------

void log(const char* msg, int rank) {
    printf("\nProc %d: %s\n", rank, msg);
}

void initFirstRandArray(int* arr, int n, int N, int min, int max) {
    int delta = max - min;
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % delta + min;
    }
    for (int i = n; i < N; i++) {
        arr[i] = 0;
    }
}

void initFirstArrayBy(int* arr, int n, int N, int val) {
    for (int i = 0; i < n; i++) {
        arr[i] = val;
    }
    for (int i = n; i < N; i++) {
        arr[i] = 0;
    }
}

void showArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void showVector(vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void showInt(int x) {
    printf("%d\n", x);
}

void showLine() {
    printf("\n");
}

int calcScalar(int* a, int* b, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += a[i] * b[i];
    }
    return result;
}




int main2(int argc, char** argv) {

    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elemsPerProc = N / size + 1;
    int extaN = elemsPerProc * size;

    int* A = new int[extaN];
    int* B = new int[extaN];

    int* chunkA = new int[elemsPerProc];
    int* chunkB = new int[elemsPerProc];

    int chunkResult;
    int result;

    // init arrays A, B
    if (rank == 0) {
        log("Init arrays", rank);
        initFirstRandArray(A, N, extaN, 0, 10);
        //initFirstArrayBy(B, N, extaN, 5);
        initFirstRandArray(B, N, extaN, 0, 10);
        showArray(A, N);
        showArray(B, N);
    }


    // send A, B to all procs
    MPI_Scatter(A, elemsPerProc, MPI_INT,chunkA, elemsPerProc, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Scatter(B, elemsPerProc, MPI_INT,chunkB, elemsPerProc, MPI_INT,0, MPI_COMM_WORLD);

    // calc chunk results
    chunkResult = calcScalar(chunkA, chunkB, elemsPerProc);
    log("scalar mult of chunk", rank);
    showArray(chunkA, elemsPerProc);
    showArray(chunkB, elemsPerProc);
    showInt(chunkResult);

   
    MPI_Reduce(&chunkResult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    // show result
    if (rank == 0) {
        log("Get scalar multiply", rank);
        showInt(result);
        showLine();
    }

    delete[] A;
    delete[] B;
    delete[] chunkA;
    delete[] chunkB;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}