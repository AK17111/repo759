#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "convolution.h"

using std::cout;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

int main(int argc, char *argv[]) {

    // Read command line arguments
    int n = std::stoi(argv[1]);
    int t = std::stoi(argv[2]);

    // Create image and output arrays
    float* image = new float[n * n];
    float* output = new float[n * n];

    // Fill image with random float values from -1 to 1
    for (int i = 0; i < n * n; i++) {
        image[i] = -1.0f + 2.0f * rand() / RAND_MAX;
    }

    // Create 3x3 mask
    float* mask = new float[3 * 3];

    // Fill mask with random float values from -1 to 1
    for (int i = 0; i < 9; i++) {
        mask[i] = -1.0f + 2.0f * rand() / RAND_MAX;
    }

    // Set number of OpenMP threads
    omp_set_num_threads(t);

    // Timer variables
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_ms;

    // Start timer
    start = high_resolution_clock::now();

    // Perform convolution
    convolve(image, output, n, mask, 3);

    // End timer
    end = high_resolution_clock::now();

    // Calculate elapsed time in milliseconds
    duration_ms =
        std::chrono::duration_cast<duration<double, std::milli>>(
            end - start
        );

    // Print first element
    cout << output[0] << "\n";

    // Print last element
    cout << output[n * n - 1] << "\n";

    // Print execution time
    cout << duration_ms.count() << "\n";

    // Free memory
    delete[] image;
    delete[] output;
    delete[] mask;

    return 0;
}