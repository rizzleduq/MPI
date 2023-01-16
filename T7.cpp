#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector> 


using namespace std;

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

inline static void log(const char* msg, int rank) {
    printf("\nProcess %d: %s\n", rank, msg);
}

inline static void initFirstRandArray(int* arr, int n, int N, int min, int max) {
    int delta = max - min;
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % delta + min;
    }
    for (int i = n; i < N; i++) {
        arr[i] = 0;
    }
}

inline static void initFirstArrayBy(int* arr, int n, int N, int val) {
    for (int i = 0; i < n; i++) {
        arr[i] = val;
    }
    for (int i = n; i < N; i++) {
        arr[i] = 0;
    }
}

inline static void showArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

inline static void showVector(vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

inline static void showInt(int x) {
    printf("%d\n", x);
}

inline static void showLine() {
    printf("\n");
}

int calcNorm(int* arr, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += abs(arr[i]);
    }
    return result;
}




int main222(int argc, char** argv) {

    int rank;
    int  size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //printf("size=%d\n", size);

    int elemsPerProc = N /  size + 1; //count of elem per proc
    int extaN = elemsPerProc *  size; // size of array for chunks

    //printf("elemperproc=%d\n", elemsPerProc);
    //printf("extan=%d\n", extaN);

    int* arr = new int[extaN];
    int* chunk = new int[elemsPerProc];

    int chunkResult;
    int result;


    
    if ( rank == 0) {
        log("Init array",  rank);
        initFirstRandArray(arr, N, extaN, -10, 11);
        showArray(arr, N);
    }


    
    MPI_Scatter(arr, elemsPerProc, MPI_INT,chunk, elemsPerProc, MPI_INT,0, MPI_COMM_WORLD);

    // calc batch res
    chunkResult = calcNorm(chunk, elemsPerProc);
    log("Get norm of chunk",  rank);
    showArray(chunk, elemsPerProc);
    showInt(chunkResult);


    // sum of batch results
    MPI_Reduce(&chunkResult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


   
    if (rank == 0) {
        log("Get array", rank);
        showInt(result);
        showLine();
    }

    delete[] chunk;
    delete[] arr;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}