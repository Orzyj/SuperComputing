#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cuda_runtime.h>
#include <string>

// Tylko deklaracja funkcji, któr¹ zawo³asz w main.cpp
std::string test();

template<typename T>
void launchCudaBubbleSort(T* d_arr, int n, double& time);