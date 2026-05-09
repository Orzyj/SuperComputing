#include "cuda.cuh"
#include <stdio.h>

__global__ void test_kernel(void) {
    printf("Hello World\n");
}

template<typename T>
__global__ void genericBubbleSortStep(T* d_arr, int n, int phase)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    int idx = (phase == 0) ? (i * 2) : (i * 2 + 1);

    if (idx + 1 < n) {
        if (d_arr[idx] > d_arr[idx + 1]) {
            T temp = d_arr[idx];
            d_arr[idx] = d_arr[idx + 1];
            d_arr[idx + 1] = temp;
        }
    }
}

std::string test() {
    test_kernel << <1, 1 >> > ();
    cudaDeviceSynchronize();
    return "Cuda GPU is Working";
}

template<typename T>
void launchCudaBubbleSort(T* d_arr, int n) {
    int threadsPerBlock = 512;
    int blocksPerGrid = ((n / 2) + threadsPerBlock - 1) / threadsPerBlock;
    if (blocksPerGrid == 0) blocksPerGrid = 1;

    for (int phase = 0; phase < n; ++phase) {
        genericBubbleSortStep<T> << <blocksPerGrid, threadsPerBlock >> > (d_arr, n, phase % 2);

        cudaDeviceSynchronize();
    }
}

template void launchCudaBubbleSort<int>(int* d_arr, int n);
template void launchCudaBubbleSort<float>(float* d_arr, int n);