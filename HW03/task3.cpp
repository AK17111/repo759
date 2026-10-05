#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include "msort.h"

using std::cout;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

int main(int argc, char *argv[]) {

    // Read command line arguments
    int n = std::stoi(argv[1]);
    int t = std::stoi(argv[2]);
    int ts = std::stoi(argv[3]);

    // Create array
    int* arr = new int[n];

    // Fill array with random integers from -1000 to 1000
    for (int i = 0; i < n; i++) {
        arr[i] = -1000 + rand() % 2001;
    }

    // Set number of OpenMP threads
    omp_set_num_threads(t);

    // Timer variables
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_ms;

    // Start timer
    start = high_resolution_clock::now();

    // Parallel merge sort
    msort(arr, n, ts);

    // End timer
    end = high_resolution_clock::now();

    // Calculate elapsed time in milliseconds
    duration_ms =
        std::chrono::duration_cast<duration<double, std::milli>>(
            end - start
        );

    // Print first element
    cout << arr[0] << "\n";

    // Print last element
    cout << arr[n - 1] << "\n";

    // Print execution time
    cout << duration_ms.count() << "\n";

    // Free memory
    delete[] arr;

    return 0;
}