#pragma once
#include <vector>
#include <algorithm>
#include <execution>

#include "cuda.cuh"
#include "ISortManager.h"

/**
 * @class BubbleSortManager
 * @brief Bubble Sort implementation of the ISortManager interface.
 * @tparam T Type of the elements in the array.
 */
template<typename T>
class BubbleSortManager : public ISortManager<T>
{
public:
    /** @brief Default constructor. */
    BubbleSortManager() = default;

    /** @brief Default destructor. */
    ~BubbleSortManager() = default;

    /**
     * @brief Sorts the array sequentially using the Bubble Sort algorithm.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using a custom std::thread and std::barrier parallel implementation.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortParallel(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using CUDA-accelerated Bubble Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void cudaSort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using OpenMP odd-even transposition parallel Bubble Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortWithOpenMP(std::vector<T>& arr, double& time) override;
};