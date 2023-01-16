#pragma once

#include <time.h>
#include <vector>
#include <iostream>

// UTILS

static inline void ProcessLog(const char* msg, int procId) {
    printf("\nProc %d: %s\n", procId, msg);
    // std::flush(std::cout);
}


int** allocateMatrix2d(size_t rows, size_t cols) {
    int** matrix = new int* [rows];
    for (size_t i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

void deallocateMatrix2d(int** matrix, size_t rows) {
    for (size_t i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;
}


void initRandMatrix(int** matrix, int min, int max, size_t N, size_t M) {
    int delta = max - min;
    srand((unsigned)time(0));

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; j++) {
            matrix[i][j] = rand() % delta + min;
        }
    }
}

void showMatrix(int** matrix, size_t N, size_t M) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void showArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void showVector(std::vector<int> v) {
    for (size_t i = 0; i < v.size(); i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}