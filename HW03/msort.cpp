#include "msort.h"
#include <algorithm>
#include <cstddef>

// Merge two sorted portions:
// arr[left ... mid-1]
// arr[mid ... right-1]
static void merge(int* arr, int* temp,
                  std::size_t left,
                  std::size_t mid,
                  std::size_t right) {

    std::size_t i = left;
    std::size_t j = mid;
    std::size_t k = left;

    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i < mid) {
        temp[k++] = arr[i++];
    }

    while (j < right) {
        temp[k++] = arr[j++];
    }

    for (std::size_t x = left; x < right; x++) {
        arr[x] = temp[x];
    }
}


// Recursive parallel merge sort
static void merge_sort(int* arr, int* temp,
                       std::size_t left,
                       std::size_t right,
                       std::size_t threshold) {

    // Number of elements in this portion
    std::size_t size = right - left;

    // If small enough, use a serial sorting algorithm
    if (size <= threshold) {
        std::sort(arr + left, arr + right);
        return;
    }

    // Find middle
    std::size_t mid = left + size / 2;

    // Sort left and right halves in parallel
    #pragma omp task shared(arr, temp)
    merge_sort(arr, temp, left, mid, threshold);

    #pragma omp task shared(arr, temp)
    merge_sort(arr, temp, mid, right, threshold);

    // Wait until both halves are sorted
    #pragma omp taskwait

    // Merge the two sorted halves
    merge(arr, temp, left, mid, right);
}


void msort(int* arr, const std::size_t n, const std::size_t threshold) {

    // Temporary array used during merging
    int* temp = new int[n];

    // Start one OpenMP parallel region
    #pragma omp parallel
    {
        // Only one thread should create the initial task
        #pragma omp single
        {
            merge_sort(arr, temp, 0, n, threshold);
        }
    }

    delete[] temp;
}