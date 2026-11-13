#include <cstdio>
#include <cstdlib>
#include <random>
#include <cuda_runtime.h>

__global__ void compute_kernel(int a, int *dA)
{
    int x = threadIdx.x;
    int y = blockIdx.x;

    int index = y * 8 + x;

    dA[index] = a * x + y;
}

int main()
{
    int *dA;
    int hA[16];

    cudaMalloc(&dA, 16 * sizeof(int));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    int a = dist(gen);

    compute_kernel<<<2, 8>>>(a, dA);

    cudaDeviceSynchronize();

    cudaMemcpy(hA, dA, 16 * sizeof(int),
               cudaMemcpyDeviceToHost);

    for (int i = 0; i < 16; i++) {
        std::printf("%d", hA[i]);

        if (i < 15) {
            std::printf(" ");
        }
    }

    std::printf("\n");

    cudaFree(dA);

    return 0;
}
