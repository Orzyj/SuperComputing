#include "cuda.cuh"
#include <stdio.h>

__global__ void test_kernel(void) {
    printf("Hello World\n");
}

std::string test() {
    // UWAGA: Poprawiona sk³adnia wywo³ania kernela!
    // Autoformatowanie VS 2022 czêsto psuje to dodaj¹c spacje: < << 1, 1 >> >
    test_kernel << <1, 1 >> > ();

    // Opcjonalnie: warto dodaæ synchronizacjê, aby upewniæ siê, ¿e printf zadzia³a przed powrotem
    cudaDeviceSynchronize();

    return "Cuda GPU is Working 13.1";
}