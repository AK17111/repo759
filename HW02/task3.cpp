#include "matmul.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

int main() {

    const unsigned int n = 1024;

    // Create A and B as 1D row-major arrays
    double* A = new double[n * n];
    double* B = new double[n * n];

    for (unsigned int i = 0; i < n * n; i++) {
        A[i] = static_cast<double>(rand()) / RAND_MAX;
        B[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    // Print number of rows
    std::cout << n << std::endl;

    // ---------------- mmul1 ----------------

    double* C1 = new double[n * n]();

    auto start = std::chrono::high_resolution_clock::now();

    mmul1(A, B, C1, n);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time1 = end - start;

    std::cout << time1.count() << std::endl;
    std::cout << C1[n * n - 1] << std::endl;


    // ---------------- mmul2 ----------------

    double* C2 = new double[n * n]();

    start = std::chrono::high_resolution_clock::now();

    mmul2(A, B, C2, n);

    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time2 = end - start;

    std::cout << time2.count() << std::endl;
    std::cout << C2[n * n - 1] << std::endl;


    // ---------------- mmul3 ----------------

    double* C3 = new double[n * n]();

    start = std::chrono::high_resolution_clock::now();

    mmul3(A, B, C3, n);

    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time3 = end - start;

    std::cout << time3.count() << std::endl;
    std::cout << C3[n * n - 1] << std::endl;


    // ---------------- mmul4 ----------------

    std::vector<double> A_vector(A, A + n * n);
    std::vector<double> B_vector(B, B + n * n);

    double* C4 = new double[n * n]();

    start = std::chrono::high_resolution_clock::now();

    mmul4(A_vector, B_vector, C4, n);

    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time4 = end - start;

    std::cout << time4.count() << std::endl;
    std::cout << C4[n * n - 1] << std::endl;


    // Deallocate memory
    delete[] A;
    delete[] B;
    delete[] C1;
    delete[] C2;
    delete[] C3;
    delete[] C4;

    return 0;
}