
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>
#include <cuda_runtime.h>
#include "vscale.cuh"

#define CUDA_CHECK(call)                                      \
    do {                                                      \
        cudaError_t err = call;                               \
        if (err != cudaSuccess) {                              \
            std::fprintf(stderr, "CUDA error: %s\n",           \
                         cudaGetErrorString(err));             \
            return 1;                                         \
        }                                                     \
    } while (0)

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        std::fprintf(stderr,
                     "Usage: ./task3 n [threads_per_block]\n");
        return 1;
    }

    unsigned int n = std::atoi(argv[1]);

    unsigned int threads = 512;

    if (argc == 3) {
        threads = std::atoi(argv[2]);
    }

    if (n == 0 || (threads != 512 && threads != 16)) {
        std::fprintf(stderr,
                     "n must be positive and threads must be 512 or 16\n");
        return 1;
    }

    // Generate random host arrays
    std::vector<float> hA(n);
    std::vector<float> hB(n);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distA(-10.0f, 10.0f);
    std::uniform_real_distribution<float> distB(0.0f, 1.0f);

    for (unsigned int i = 0; i < n; i++) {
        hA[i] = distA(gen);
        hB[i] = distB(gen);
    }

    // Allocate device arrays
    float *dA;
    float *dB;

    CUDA_CHECK(cudaMalloc((void**)&dA, n * sizeof(float)));
    CUDA_CHECK(cudaMalloc((void**)&dB, n * sizeof(float)));

    // Copy arrays to device
    CUDA_CHECK(cudaMemcpy(dA, hA.data(),
                         n * sizeof(float),
                         cudaMemcpyHostToDevice));

    CUDA_CHECK(cudaMemcpy(dB, hB.data(),
                         n * sizeof(float),
                         cudaMemcpyHostToDevice));

    // Execution configuration
    unsigned int blocks = (n + threads - 1) / threads;

    // CUDA events for timing
    cudaEvent_t start, stop;

    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // Launch kernel and measure execution time
    CUDA_CHECK(cudaEventRecord(start));

    vscale<<<blocks, threads>>>(dA, dB, n);

    CUDA_CHECK(cudaGetLastError());

    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));

    float milliseconds = 0.0f;

    CUDA_CHECK(cudaEventElapsedTime(&milliseconds,
                                   start, stop));

    // Copy result back
    CUDA_CHECK(cudaMemcpy(hB.data(), dB,
                         n * sizeof(float),
                         cudaMemcpyDeviceToHost));

    // Print required output
    std::printf("%.6f\n", milliseconds);
    std::printf("%.9f\n", hB[0]);
    std::printf("%.9f\n", hB[n - 1]);

    // Cleanup
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));

    CUDA_CHECK(cudaFree(dA));
    CUDA_CHECK(cudaFree(dB));

    return 0;
}
