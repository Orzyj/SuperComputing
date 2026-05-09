#pragma once
#include <vector>
#include <algorithm>
#include <execution>

#include "cuda.cuh"

class BubbleSortManager
{
public:
	BubbleSortManager() = default;
	~BubbleSortManager() = default;

	template<typename T>
	void sort(std::vector<T>& arr, double& time);

	template<typename T>
	void sortParallel(std::vector<T>& arr, double& time);

	template<typename T>
	void cudaSort(std::vector<T>& arr, double& time);

	template<typename T>
	void sortWithOpenMP(std::vector<T>& arr, double& time);

};