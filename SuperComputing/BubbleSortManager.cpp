#include "BubbleSortManager.h"
#include "cuda.cuh" // Wymagane do wywo³ania launchCudaBubbleSort

#include <chrono>
#include <ctime>
#include <thread>
#include <algorithm>
#include <barrier>
#include <vector>
#include <omp.h>

template<typename T>
void BubbleSortManager::sortWithOpenMP(std::vector<T>& arr, double& time) {
    int n = arr.size();

    if (n < 2) {
        time = 0.0;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    for (int phase = 0; phase < n; ++phase) {
        if (phase % 2 == 0) {
            #pragma omp for
            for (int i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                }
            }
        }
        else {
            #pragma omp for
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    time = duration.count();
}

template<typename T>
void BubbleSortManager::sort(std::vector<T>& arr, double& time)
{
    int n = arr.size();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    time = duration.count();
}

template<typename T>
void BubbleSortManager::sortParallel(std::vector<T>& arr, double& time)
{
    int n = arr.size();

    if (n < 2) {
        time = 0.0;
        return;
    }

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;
    if (numThreads > n / 2) numThreads = n / 2;

    std::barrier sync_point(numThreads);

    auto worker = [&](int thread_id) {
        int total_pairs = n / 2;
        int pairs_per_thread = total_pairs / numThreads;
        int start_pair = thread_id * pairs_per_thread;
        int end_pair = (thread_id == numThreads - 1) ? total_pairs : start_pair + pairs_per_thread;

        for (int phase = 0; phase < n; ++phase) {
            if (phase % 2 == 0) {
                for (int p = start_pair; p < end_pair; ++p) {
                    int idx = p * 2;
                    if (idx + 1 < n && arr[idx] > arr[idx + 1]) {
                        std::swap(arr[idx], arr[idx + 1]);
                    }
                }
            }
            else {
                for (int p = start_pair; p < end_pair; ++p) {
                    int idx = p * 2 + 1;
                    if (idx + 1 < n && arr[idx] > arr[idx + 1]) {
                        std::swap(arr[idx], arr[idx + 1]);
                    }
                }
            }
            sync_point.arrive_and_wait();
        }
    };

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker, i);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    time = duration.count();
}

template<typename T>
void BubbleSortManager::cudaSort(std::vector<T>& arr, double& time)
{
    int n = arr.size();
    if (n == 0) {
        time = 0.0;
        return;
    }

    T* d_arr = nullptr;
    size_t sizeInBytes = n * sizeof(T);

    cudaMalloc((void**)&d_arr, sizeInBytes);
    cudaMemcpy(d_arr, arr.data(), sizeInBytes, cudaMemcpyHostToDevice);
    launchCudaBubbleSort(d_arr, n, time);
    cudaMemcpy(arr.data(), d_arr, sizeInBytes, cudaMemcpyDeviceToHost);
    cudaFree(d_arr);
}

template void BubbleSortManager::sort<int>(std::vector<int>&, double&);
template void BubbleSortManager::sort<float>(std::vector<float>&, double&);

template void BubbleSortManager::sortParallel<int>(std::vector<int>&, double&);
template void BubbleSortManager::sortParallel<float>(std::vector<float>&, double&);

template void BubbleSortManager::cudaSort<int>(std::vector<int>&, double&);
template void BubbleSortManager::cudaSort<float>(std::vector<float>&, double&);

template void BubbleSortManager::sortWithOpenMP<int>(std::vector<int>&, double&);
template void BubbleSortManager::sortWithOpenMP<float>(std::vector<float>&, double&);