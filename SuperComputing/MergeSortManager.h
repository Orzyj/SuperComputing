#pragma once
#include "ISortManager.h"
#include <vector>

/**
 * @class MergeSortManager
 * @brief Merge Sort implementation of the ISortManager interface.
 * @tparam T Type of the elements in the array.
 */
template <typename T>
class MergeSortManager : public ISortManager<T>
{
public:
    /** @brief Default constructor. */
    MergeSortManager() = default;

    /** @brief Default destructor. */
    ~MergeSortManager() = default;

    /**
     * @brief Sorts the array sequentially using the Merge Sort algorithm.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using std::async parallel Merge Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortParallel(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using CUDA-accelerated Merge Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void cudaSort(std::vector<T>& arr, double& time) override;

    /**
     * @brief Sorts the array using OpenMP task-based parallel Merge Sort.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    void sortWithOpenMP(std::vector<T>& arr, double& time) override;
};