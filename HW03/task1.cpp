#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "matmul.h"

using std::cout;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

int main(int argc, char *argv[]) {

    // Read command line arguments
    int n = std::stoi(argv[1]);
    int t = std::stoi(argv[2]);

    // Create matrices A, B, and C
    float* A = new float[n * n];
    float* B = new float[n * n];
    float* C = new float[n * n];

    // Fill A and B with random float values from -1 to 1
    for (int i = 0; i < n * n; i++) {
        A[i] = -1.0f + 2.0f * rand() / RAND_MAX;
        B[i] = -1.0f + 2.0f * rand() / RAND_MAX;
        C[i] = 0.0f;
    }

    // Set number of OpenMP threads
    omp_set_num_threads(t);

    // Timer variables
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_ms;

    // Start timer
    start = high_resolution_clock::now();

    // Matrix multiplication
    mmul(A, B, C, n);

    // End timer
    end = high_resolution_clock::now();

    // Calculate elapsed time in milliseconds
    duration_ms =
        std::chrono::duration_cast<duration<double, std::milli>>(end - start);

    // Print first element of C
    cout << C[0] << "\n";

    // Print last element of C
    cout << C[n * n - 1] << "\n";

    // Print time in milliseconds
    cout << duration_ms.count() << "\n";

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}