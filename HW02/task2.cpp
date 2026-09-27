#include "convolution.h"
#include <iostream>
#include <cstdlib>
#include <chrono>

int main(int argc, char *argv[]) {

    std::size_t n = std::stoul(argv[1]);
    std::size_t m = std::stoul(argv[2]);

    // Create n x n image
    float *image = new float[n * n];

    for (std::size_t i = 0; i < n * n; i++) {
        image[i] = -10.0f +
                   20.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    // Create m x m mask
    float *mask = new float[m * m];

    for (std::size_t i = 0; i < m * m; i++) {
        mask[i] = -1.0f +
                  2.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    // Create output array
    float *output = new float[n * n];

    // Time the convolution
    auto start = std::chrono::high_resolution_clock::now();

    convolve(image, output, n, mask, m);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;

    // Print time
    std::cout << elapsed.count() << std::endl;

    // Print first element
    std::cout << output[0] << std::endl;

    // Print last element
    std::cout << output[n * n - 1] << std::endl;

    // Deallocate memory
    delete[] image;
    delete[] mask;
    delete[] output;

    return 0;
}