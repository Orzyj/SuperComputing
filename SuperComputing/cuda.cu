#include "cuda.cuh"
#include <stdio.h>

__global__ void test_kernel(void) {
    printf("Hello World\n");
}

template<typename T>
__global__ void genericBubbleSortStep(T* d_arr, int n, int phase){
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

template<typename T>
__global__ void genericQuickSortKernel(T* d_arr, int left, int right) {
    if (left >= right) return;

    T pivot = d_arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (d_arr[j] <= pivot) {
            i++;
            T temp = d_arr[i];
            d_arr[i] = d_arr[j];
            d_arr[j] = temp;
        }
    }
    T temp = d_arr[i + 1];
    d_arr[i + 1] = d_arr[right];
    d_arr[right] = temp;

    int pivotIdx = i + 1;

    if (left < pivotIdx - 1) {
        cudaStream_t s1;
        cudaStreamCreateWithFlags(&s1, cudaStreamNonBlocking);
        genericQuickSortKernel<T> << <1, 1, 0, s1 >> > (d_arr, left, pivotIdx - 1);
        cudaStreamDestroy(s1);
    }
    if (pivotIdx + 1 < right) {
        cudaStream_t s2;
        cudaStreamCreateWithFlags(&s2, cudaStreamNonBlocking);
        genericQuickSortKernel<T> << <1, 1, 0, s2 >> > (d_arr, pivotIdx + 1, right);
        cudaStreamDestroy(s2);
    }
}

template<typename T>
__global__ void genericMergeSortStep(T* source, T* dest, int n, int width) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int left = i * 2 * width;

    if (left >= n) return;

    int mid = left + width;
    if (mid > n) mid = n;

    int right = left + 2 * width;
    if (right > n) right = n;

    int l = left;
    int r = mid;
    int idx = left;

    while (l < mid && r < right) {
        if (source[l] <= source[r]) {
            dest[idx++] = source[l++];
        }
        else {
            dest[idx++] = source[r++];
        }
    }

    while (l < mid) {
        dest[idx++] = source[l++];
    }

    while (r < right) {
        dest[idx++] = source[r++];
    }
}

std::string test() {
    test_kernel << <1, 1 >> > ();
    cudaDeviceSynchronize();
    return "Cuda GPU is Working";
}

template<typename T>
void launchCudaBubbleSort(T* d_arr, int n, double& time) {
    int threadsPerBlock = 512;
    int blocksPerGrid = ((n / 2) + threadsPerBlock - 1) / threadsPerBlock;
    if (blocksPerGrid == 0) blocksPerGrid = 1;

    auto start = std::chrono::high_resolution_clock::now();
    for (int phase = 0; phase < n; ++phase) {
        genericBubbleSortStep<T> << <blocksPerGrid, threadsPerBlock >> > (d_arr, n, phase % 2);

        cudaDeviceSynchronize();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    time = duration.count();
}

template<typename T>
void launchCudaMergeSort(T* d_arr, int n, double& time) {
    if (n <= 1) return;

    T* d_temp;
    cudaMalloc((void**)&d_temp, n * sizeof(T));

    T* A = d_arr;
    T* B = d_temp;

    auto start = std::chrono::high_resolution_clock::now();
    for (int width = 1; width < n; width *= 2) {
        int num_merges = (n + (2 * width) - 1) / (2 * width);
        int threadsPerBlock = 256;
        int blocksPerGrid = (num_merges + threadsPerBlock - 1) / threadsPerBlock;

        genericMergeSortStep<T> << <blocksPerGrid, threadsPerBlock >> > (A, B, n, width);
        cudaDeviceSynchronize();

        T* temp = A;
        A = B;
        B = temp;
    }

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();

    if (A == d_temp) {
        cudaMemcpy(d_arr, d_temp, n * sizeof(T), cudaMemcpyDeviceToDevice);
    }

    cudaFree(d_temp);
}

template<typename T>
void launchCudaQuickSort(T* d_arr, int n, double& time) {
    if (n <= 1) {
        time = 0.0;
        return;
    }

    cudaDeviceSetLimit(cudaLimitDevRuntimeSyncDepth, 24);

    auto start = std::chrono::high_resolution_clock::now();

    genericQuickSortKernel<T> << <1, 1 >> > (d_arr, 0, n - 1);

    cudaDeviceSynchronize();

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template void launchCudaBubbleSort<int>(int*, int, double&);
template void launchCudaBubbleSort<float>(float*, int, double&);

template void launchCudaMergeSort<int>(int*, int, double&);
template void launchCudaMergeSort<float>(float*, int, double&);

template void launchCudaQuickSort<int>(int*, int, double&);
template void launchCudaQuickSort<float>(float*, int, double&);