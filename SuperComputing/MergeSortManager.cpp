#include "MergeSortManager.h"
#include "cuda.cuh" // Nag³ówek dla wywo³añ CUDA

#include <chrono>
#include <algorithm>
#include <future>
#include <thread>
#include <omp.h>

template<typename T>
void mergeSortSeq(std::vector<T>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    mergeSortSeq(arr, left, mid);
    mergeSortSeq(arr, mid + 1, right);

    std::inplace_merge(arr.begin() + left, arr.begin() + mid + 1, arr.begin() + right + 1);
}

template<typename T>
void mergeSortParallelHelper(std::vector<T>& arr, int left, int right, int depth) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    if (depth > 0) {
        auto fut = std::async(std::launch::async, mergeSortParallelHelper<T>, std::ref(arr), left, mid, depth - 1);
        mergeSortParallelHelper(arr, mid + 1, right, depth - 1);

        fut.get();
    }
    else {
        mergeSortSeq(arr, left, mid);
        mergeSortSeq(arr, mid + 1, right);
    }

    std::inplace_merge(arr.begin() + left, arr.begin() + mid + 1, arr.begin() + right + 1);
}

template<typename T>
void mergeSortOmpHelper(std::vector<T>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    #pragma omp task shared(arr) if(right - left > 10000)
    mergeSortOmpHelper(arr, left, mid);

    #pragma omp task shared(arr) if(right - left > 10000)
    mergeSortOmpHelper(arr, mid + 1, right);

    #pragma omp taskwait 
    std::inplace_merge(arr.begin() + left, arr.begin() + mid + 1, arr.begin() + right + 1);
}

template<typename T>
void MergeSortManager<T>::sort(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }

    auto start = std::chrono::high_resolution_clock::now();

    mergeSortSeq(arr, 0, arr.size() - 1);

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void MergeSortManager<T>::sortParallel(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }

    auto start = std::chrono::high_resolution_clock::now();
    int numThreads = std::thread::hardware_concurrency();
    int maxDepth = 0;
    while (numThreads > 0) { maxDepth++; numThreads >>= 1; }

    mergeSortParallelHelper(arr, 0, arr.size() - 1, maxDepth);

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void MergeSortManager<T>::sortWithOpenMP(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
    #pragma omp single
        {
            mergeSortOmpHelper(arr, 0, arr.size() - 1);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void MergeSortManager<T>::cudaSort(std::vector<T>& arr, double& time) {
    int n = arr.size();

    if (n == 0) { 
        time = 0.0; return; 
    }

    T* d_arr = nullptr;
    size_t sizeInBytes = n * sizeof(T);

    cudaMalloc((void**)&d_arr, sizeInBytes);
    cudaMemcpy(d_arr, arr.data(), sizeInBytes, cudaMemcpyHostToDevice);

    launchCudaMergeSort(d_arr, n, time);

    cudaMemcpy(arr.data(), d_arr, sizeInBytes, cudaMemcpyDeviceToHost);
    cudaFree(d_arr);
}

template class MergeSortManager<int>;
template class MergeSortManager<float>;