
// CudaTestRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <random>
#include "cuda_kernel.cuh"

void populate_vector(std::vector<double>* vec_in, double bottom_thresh = 0, double top_thresh = 1) {
    if (bottom_thresh > top_thresh) {
        double foo = top_thresh;
        top_thresh = bottom_thresh;
        bottom_thresh = foo;
    }
    std::random_device double_seed;
    std::mt19937 generator(double_seed());
    std::uniform_real_distribution<double> dist{bottom_thresh, top_thresh};

    auto gen = [&dist, &generator]() {
            return dist(generator);
        };

    std::generate(begin(*vec_in), end(*vec_in), gen);
}

template<typename T>
void print_vector(std::vector<T>* vec) {
    for (int i = 0; i < vec->size(); i++) {
        std::cout << vec->at(i) << " ";
    }
    std::cout << std::endl;
    return;
}

template<typename T>
std::vector<T> add_vector(std::vector<T>* vec_one, std::vector<T>* vec_two) {
    size_t size = (vec_one->size() >= vec_two->size()) ? vec_one->size() : vec_two->size();

    std::vector<T> vec_out(size);

    for (int i = 0; i < size; i++) {
        vec_out[i] = vec_one->at(i) + vec_two->at(i);
    }

    return vec_out;
}

int main()
{
    //define array size
    int arrs = 50;
    double val_range[] = {0, 1000};
    
    // Initialize arrays A, B, and C.
    double A[3], B[3], C[3];
    std::vector<double> vec_a(arrs);
    std::vector<double> vec_b(arrs);

    // Populate arrays A and B.
    A[0] = 5; A[1] = 8; A[2] = 3;
    B[0] = 7; B[1] = 6; B[2] = 4;
    populate_vector(&vec_a, val_range[0], val_range[1]);
    populate_vector(&vec_b, val_range[0], val_range[1]);

    //print vectors
    std::cout << "A: ";
    print_vector(&vec_a);
    std::cout << "B: ";
    print_vector(&vec_b);


    // Sum array elements across ( C[0] = A[0] + B[0] ) into array C using CUDA.
    cuda_sum(A, B, C, 3);
    std::vector<double> vec_c = add_vector(&vec_a, &vec_b);
    std::vector<double> vec_c_cuda(vec_a.size());

    // Print out result.
    std::cout << "C = " << C[0] << ", " << C[1] << ", " << C[2] << std::endl;
    std::cout << "C: ";
    print_vector(&vec_c);
    std::cin.get();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file