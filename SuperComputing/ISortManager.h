#pragma once
#include <vector>

/**
 * @class ISortManager
 * @brief Common interface for all sorting algorithms.
 * @tparam T Type of the elements in the array.
 */
template <typename T>
class ISortManager
{
public:
    /** @brief Virtual destructor. */
    virtual ~ISortManager() = default;

    /**
     * @brief Sorts the array sequentially on a single CPU thread.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    virtual void sort(std::vector<T>& arr, double& time) = 0;

    /**
     * @brief Sorts the array using standard C++ multithreading.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    virtual void sortParallel(std::vector<T>& arr, double& time) = 0;

    /**
     * @brief Sorts the array using CUDA GPU acceleration.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    virtual void cudaSort(std::vector<T>& arr, double& time) = 0;

    /**
     * @brief Sorts the array using OpenMP multithreading.
     * @param arr Vector of elements to be sorted.
     * @param time Output parameter to store execution time (in ms).
     */
    virtual void sortWithOpenMP(std::vector<T>& arr, double& time) = 0;
};