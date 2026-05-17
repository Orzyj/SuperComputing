# SuperComputing - Sorting Benchmark

![C++](https://img.shields.io/badge/C%2B%2B-17%2F20-blue?style=flat-square&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-Widgets-41CD52?style=flat-square&logo=qt)
![NVIDIA CUDA](https://img.shields.io/badge/NVIDIA-CUDA-76B900?style=flat-square&logo=nvidia)
![OpenMP](https://img.shields.io/badge/OpenMP-Multi--Processing-FF9900?style=flat-square)
![Python](https://img.shields.io/badge/Python-Data%20Analysis-3776AB?style=flat-square&logo=python)

## Project Overview
A desktop application developed within the Qt/C++ ecosystem designed to perform rigorous performance benchmarking of select sorting algorithms (**Bubble Sort**, **Merge Sort**, and **Quick Sort**). The system measures execution times across `int` and `float` data structures as a function of dataset size, distributing the workloads across diverse hardware and software architectures.

## Supported Execution Variants
* **Sequential CPU:** A single-threaded baseline executed on a single core of the processor.
* **Multithreaded CPU (C++20):** An implementation leveraging asynchronous concurrency mechanisms (`std::thread` / `std::async`).
* **OpenMP:** Multi-processing paradigm based on compiler directives for shared-memory systems.
* **NVIDIA CUDA:** Massively parallel acceleration handled directly on the graphics processing unit (GPU) cores.

## Key Features
* Asynchronous, non-blocking Graphical User Interface (Qt Widgets).
* Dynamic rendering and visualization of sorted array slices.
* Automated logging of raw timing metrics into structured CSV files.
* Integration with a Python analytical pipeline (`pandas`, `matplotlib`, `seaborn`) for automatic generation of 2D linear and 3D spatial plots.
* Dedicated module for synthesizing and exporting comprehensive performance reports in PDF format.
