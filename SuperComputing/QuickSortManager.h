#pragma once
#include "ISortManager.h"
#include <vector>

/**
 * @class QuickSortManager
 * @brief Quick Sort implementation of the ISortManager interface.
 * @tparam T Type of the elements in the array.
 */
template <typename T>
class QuickSortManager : public ISortManager<T>
{
public:
    /** @brief Default constructor. */
    QuickSortManager() = default;

    /** @brief Default destructor. */
    ~QuickSortManager() = default;

    /**
     * @brief Sorts the array sequentially using the Quick Sort algorithm.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using std::thread/std::async parallel Quick Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortParallel(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using CUDA-accelerated Quick Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void cudaSort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using OpenMP parallel Quick Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortWithOpenMP(std::vector<T>& arr, double& time) override;
};