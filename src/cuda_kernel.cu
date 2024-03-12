#include "cuda_runtime.h"
#include "cuda_kernel.cuh"
#include <vector>

__global__ void vectorAdditionKernel(double* A, double* B, double* C, int arraySize) {
    // Get thread ID.
    int threadID = blockDim.x * blockIdx.x + threadIdx.x;

    // Check if thread is within array bounds.
    if (threadID < arraySize) {
        // Add a and b.
        C[threadID] = A[threadID] + B[threadID];
    }
}



/**
 * Wrapper function for the CUDA kernel function.
 * @param A Array A.
 * @param B Array B.
 * @param C Sum of array elements A and B directly across.
 * @param arraySize Size of arrays A, B, and C.
 */
void cuda_sum(double* A, double* B, double* C, int arraySize) {

    // Initialize device pointers.
    double* d_A, * d_B, * d_C;

    // Allocate device memory.
    cudaMalloc((void**)&d_A, arraySize * sizeof(double));
    cudaMalloc((void**)&d_B, arraySize * sizeof(double));
    cudaMalloc((void**)&d_C, arraySize * sizeof(double));

    // Transfer arrays a and b to device.
    cudaMemcpy(d_A, A, arraySize * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, arraySize * sizeof(double), cudaMemcpyHostToDevice);

    // Calculate blocksize and gridsize.
    dim3 blockSize(512, 1, 1);
    dim3 gridSize(512 / arraySize + 1, 1);

    // Launch CUDA kernel.
    vectorAdditionKernel <<<gridSize, blockSize >>> (d_A, d_B, d_C, arraySize);

    // Copy result array c back to host memory.
    cudaMemcpy(C, d_C, arraySize * sizeof(double), cudaMemcpyDeviceToHost);

}

void cuda_vecsum(std::vector<double>* A, std::vector<double>* B, std::vector<double>* C) {

    // Initialize device pointers.
    double* d_A, * d_B, * d_C;

    // Allocate device memory.
    cudaMalloc((void**)&d_A, A->size() * sizeof(double));
    cudaMalloc((void**)&d_B, A->size() * sizeof(double));
    cudaMalloc((void**)&d_C, A->size() * sizeof(double));

    // Transfer arrays a and b to device.
    cudaMemcpy(d_A, A, A->size() * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, A->size() * sizeof(double), cudaMemcpyHostToDevice);

    // Calculate blocksize and gridsize.
    dim3 blockSize(512, 1, 1);
    dim3 gridSize(512 / A->size() + 1, 1);

    // Launch CUDA kernel.
    vectorAdditionKernel <<<gridSize, blockSize >>> (d_A, d_B, d_C, A->size());

    //temporary value
    //double* C_temp = C->data();

    // Copy result array c back to host memory.
    cudaMemcpy(C, d_C, A->size() * sizeof(double), cudaMemcpyDeviceToHost);
    //C->assign(C_temp, C_temp + C->size());

}