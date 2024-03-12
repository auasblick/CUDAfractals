
// CudaTestRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
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
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    try {
        for (int i = 0; i < vec->size(); i++) {
            std::cout << vec->at(i) << " ";
        }
    std::cout << std::endl;
    }
    catch (std::exception e) {
        std::cout << "Printing failed\n";
    }
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

template<typename T>
bool compare_vectors(std::vector<T>* vec_one, std::vector<T>* vec_two, T tolerance = 0) {
    if (vec_one->size() != vec_two->size()) {
        return false;
    }
    try{
        for (int i = 0; i < vec_one->size(); i++) {
            if (std::abs((*vec_one)[i] - (*vec_two)[i]) <= tolerance) {
                return false;
            }
        }

    }
    catch (std::exception e) {
        try {
            (*vec_one)[vec_one->size()];
        }
        catch (std::exception e) {
            std::cout << "Read access violation for first comparison vector\n";
        }
        try {
            (*vec_two)[vec_one->size()];
        }
        catch (std::exception e) {
            std::cout << "Read access violation for second comparison vector\n";
        }
    }
    return true;
}

int main()
{
    //define array size
    int arrs = 5;
    double val_range[] = {0, 1000};
    
    // Initialize arrays A, B, and C.
    double A[3], B[3], C[3];
    std::vector<double> vec_a(arrs);
    std::vector<double> vec_b(arrs);

    // Populate arrays A and B.
    A[0] = 5.3; A[1] = 8; A[2] = 3.9;
    B[0] = 7; B[1] = 6.1; B[2] = 4.2;
    cuda_sum(A, B, C, 3);
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            std::cout << "A\tB\tC\n";
        }
        std::cout << A[i] << "\t" << B[i] << "\t" << C[i] << "\n";
    }
    std::cout << "\n\n\n";
    populate_vector(&vec_a, val_range[0], val_range[1]);
    populate_vector(&vec_b, val_range[0], val_range[1]);

    //print vectors
    std::cout << "A: ";
    print_vector(&vec_a);
    std::cout << "B: ";
    print_vector(&vec_b);


    // Sum array elements across ( C[i] = A[i] + B[i] ) into array C using CUDA.
    std::vector<double> vec_c = add_vector(&vec_a, &vec_b);
    std::vector<double> vec_c_cuda(vec_a.size());
    cuda_vecsum(&vec_a, &vec_b, &vec_c_cuda);

    // Print out result.
    if (compare_vectors(&vec_c, &vec_c_cuda)) {
        std::cout << "\nSuccuess! c and c_cuda are identical\n";
    }
    else {
        std::cout << "\nFailure\n";
        std::cout << "C: ";
        print_vector(&vec_c);
        std::cout << "C (cuda): ";
        print_vector(&vec_c_cuda);
    }


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