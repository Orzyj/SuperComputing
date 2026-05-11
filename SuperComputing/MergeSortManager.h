#pragma once
#include <vector>

class MergeSortManager
{
public:
	MergeSortManager() = default;
	~MergeSortManager() = default;

	template<typename T>
	void sort(std::vector<T>& arr, double& time);

	template<typename T>
	void sortParallel(std::vector<T>& arr, double& time);

	template<typename T>
	void cudaSort(std::vector<T>& arr, double& time);

	template<typename T>
	void sortWithOpenMP(std::vector<T>& arr, double& time);
};

