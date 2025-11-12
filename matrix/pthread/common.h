#pragma once
#define NOMINMAX
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>

using Matrix = std::vector<std::vector<double>>;

void fill_matrix(Matrix& m, int N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            m[i][j] = dis(gen);
        }
    }
}

Matrix create_matrix(int N) {
    return Matrix(N, std::vector<double>(N, 0.0));
}


void sequential_multiply(const Matrix& A, const Matrix& B, Matrix& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0;
            for (int m = 0; m < N; ++m) {
                sum += A[i][m] * B[m][j];
            }
            C[i][j] = sum;
        }
    }
}

void compute_result_block(const Matrix& A, const Matrix& B, Matrix& C,
                          int N, int k, int i_block, int j_block) {
    
    int row_start = i_block * k;
    int row_end = std::min((i_block + 1) * k, N);
    int col_start = j_block * k;
    int col_end = std::min((j_block + 1) * k, N);

    int num_blocks_dim = (N + k - 1) / k;

    for (int i = row_start; i < row_end; ++i) {
        for (int j = col_start; j < col_end; ++j) {
            
            double sum = 0;
            
            for (int m_block = 0; m_block < num_blocks_dim; ++m_block) {
                int m_start = m_block * k;
                int m_end = std::min((m_block + 1) * k, N);
                
                for (int m = m_start; m < m_end; ++m) {
                    sum += A[i][m] * B[m][j];
                }
            }
            C[i][j] = sum;
        }
    }
}

class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        return duration.count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
};