#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cuda_runtime.h>
#include <string>

std::string test();

template<typename T>
void launchCudaBubbleSort(T* d_arr, int n, double& time);

template<typename T>
void launchCudaMergeSort(T* d_arr, int n, double& time);

template<typename T>
void launchCudaQuickSort(T* d_arr, int n, double& time);