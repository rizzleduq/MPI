#pragma once

#include "stdlib.h"
#include "math.h"
#include <iostream>
#include <ctime>
#include <type_traits>
using namespace std;


#define RANDOM_SPREAD_COEFFICIENT 3

template <typename T, typename = std::enable_if<
    std::is_same<T, int>::value ||
    std::is_same<T, double>::value ||
    std::is_same<T, float>::value>
    ::type>
    void rand_vec(T* vec, size_t size)
{
    if (!vec) {
        return;
    }
    for (size_t i = 0; i < size; ++i)
    {
        vec[i] = rand() % ((int)size * RANDOM_SPREAD_COEFFICIENT) + 1;
    }
    return;
}