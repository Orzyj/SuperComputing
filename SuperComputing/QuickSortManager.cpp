#include "QuickSortManager.h"
#include "cuda.cuh" // Nag³ówek dla wywo³añ CUDA dla QuickSort

#include <chrono>
#include <algorithm>
#include <future>
#include <thread>
#include <omp.h>

// --- Funkcje pomocnicze (Partition) ---
template<typename T>
int partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// --- QuickSort Sekwencyjny ---
template<typename T>
void quickSortSeq(std::vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortSeq(arr, low, pi - 1);
        quickSortSeq(arr, pi + 1, high);
    }
}

// --- QuickSort Parallel (std::async) ---
template<typename T>
void quickSortParallelHelper(std::vector<T>& arr, int low, int high, int depth) {
    if (low >= high) return;

    int pi = partition(arr, low, high);

    if (depth > 0) {
        auto fut = std::async(std::launch::async, quickSortParallelHelper<T>, std::ref(arr), low, pi - 1, depth - 1);
        quickSortParallelHelper(arr, pi + 1, high, depth - 1);
        fut.get();
    }
    else {
        quickSortSeq(arr, low, pi - 1);
        quickSortSeq(arr, pi + 1, high);
    }
}

// --- QuickSort OpenMP ---
template<typename T>
void quickSortOmpHelper(std::vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

#pragma omp task shared(arr) if(high - low > 10000)
        quickSortOmpHelper(arr, low, pi - 1);

#pragma omp task shared(arr) if(high - low > 10000)
        quickSortOmpHelper(arr, pi + 1, high);
    }
}

// --- Implementacja metod klasy ---

template<typename T>
void QuickSortManager::sort(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }
    auto start = std::chrono::high_resolution_clock::now();

    quickSortSeq(arr, 0, arr.size() - 1);

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void QuickSortManager::sortParallel(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }
    auto start = std::chrono::high_resolution_clock::now();

    int numThreads = std::thread::hardware_concurrency();
    int maxDepth = 0;
    while (numThreads > 0) { maxDepth++; numThreads >>= 1; }

    quickSortParallelHelper(arr, 0, arr.size() - 1, maxDepth);

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void QuickSortManager::sortWithOpenMP(std::vector<T>& arr, double& time) {
    if (arr.empty()) { time = 0.0; return; }
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        #pragma omp single
        {
            quickSortOmpHelper(arr, 0, arr.size() - 1);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration<double, std::milli>(end - start).count();
}

template<typename T>
void QuickSortManager::cudaSort(std::vector<T>& arr, double& time) {
    int n = arr.size();
    if (n <= 1) { time = 0.0; return; }

    T* d_arr = nullptr;
    size_t sizeInBytes = n * sizeof(T);

    cudaMalloc((void**)&d_arr, sizeInBytes);
    cudaMemcpy(d_arr, arr.data(), sizeInBytes, cudaMemcpyHostToDevice);

    launchCudaQuickSort(d_arr, n, time);

    cudaMemcpy(arr.data(), d_arr, sizeInBytes, cudaMemcpyDeviceToHost);
    cudaFree(d_arr);
}

// --- Eksplicytna konkretyzacja szablonów ---
template void QuickSortManager::sort<int>(std::vector<int>&, double&);
template void QuickSortManager::sort<float>(std::vector<float>&, double&);

template void QuickSortManager::sortParallel<int>(std::vector<int>&, double&);
template void QuickSortManager::sortParallel<float>(std::vector<float>&, double&);

template void QuickSortManager::cudaSort<int>(std::vector<int>&, double&);
template void QuickSortManager::cudaSort<float>(std::vector<float>&, double&);

template void QuickSortManager::sortWithOpenMP<int>(std::vector<int>&, double&);
template void QuickSortManager::sortWithOpenMP<float>(std::vector<float>&, double&);