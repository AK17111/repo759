#include "scan.h"

template <typename T>
T* scan(const T* input, int n)
{
    T* output = new T[n];

    if (n > 0)
    {
        output[0] = input[0];

        for (int i = 1; i < n; i++)
        {
            output[i] = output[i - 1] + input[i];
        }
    }

    return output;
}

template float* scan(const float* input, int n);
template int* scan(const int* input, int n);