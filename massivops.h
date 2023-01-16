#pragma once

#include "stdlib.h"
#include "math.h"
#include <type_traits>

template <typename T, typename = std::enable_if<
    std::is_same<T, int>::value ||
    std::is_same<T, double>::value ||
    std::is_same<T, float>::value>
    ::type>
    void sum_mass(T* a, T* b, int size) {
    for (int i = 0; i < size; ++i) {
        a[i] = a[i] + b[i];
    }
}

template <typename T, typename = std::enable_if<
    std::is_same<T, int>::value ||
    std::is_same<T, double>::value ||
    std::is_same<T, float>::value>
    ::type>
    void multiply_mass(T* a, T* b, int size) {
    for (int i = 0; i < size; ++i) {
        a[i] = a[i] * b[i];
    }
}