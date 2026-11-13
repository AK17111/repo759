#include <cstdio>
#include <cuda_runtime.h>

__global__ void factorial_kernel()
{
    int a = threadIdx.x + 1;
    int b = 1;

    for (int i = 1; i <= a; i++) {
        b *= i;
    }

    std::printf("%d!=%d\n", a, b);
}

int main()
{
    factorial_kernel<<<1, 8>>>();

    cudaDeviceSynchronize();

    return 0;
}
