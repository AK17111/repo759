#include "matmul.h"

void mmul1(const double* A, const double* B, double* C, const unsigned int n) {

    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            for (unsigned int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}