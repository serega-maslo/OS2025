#define NOMINMAX
#include <thread>
#include "common.h"

const int N = 60;

int main() {
    Matrix A = create_matrix(N);
    Matrix B = create_matrix(N);
    Matrix C_seq = create_matrix(N);
    Matrix C_threaded = create_matrix(N);

    fill_matrix(A, N);
    fill_matrix(B, N);

    std::cout << "Matrix Multiplication Benchmark (N = " << N << ")" << std::endl;
    std::cout << "Using <thread>\n" << std::endl;

    Timer timer_seq;
    sequential_multiply(A, B, C_seq, N);
    double seq_time = timer_seq.elapsed_ms();
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Sequential (1 Thread) Time: " << seq_time << " ms" << std::endl;
    std::cout << "------------------------------------------------\n" << std::endl;
    std::cout << std::setw(10) << "Block K" << std::setw(15) << "Num Threads" << std::setw(15) << "Time (ms)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int k = 1; k <= N; ++k) {
        int num_blocks_dim = (N + k - 1) / k;
        int num_threads = num_blocks_dim * num_blocks_dim;

        std::vector<std::thread> threads;

        Timer timer_threaded;

        for (int i_block = 0; i_block < num_blocks_dim; ++i_block) {
            for (int j_block = 0; j_block < num_blocks_dim; ++j_block) {
                threads.emplace_back([&, i_block, j_block]() {
                    compute_result_block(A, B, C_threaded, N, k, i_block, j_block);
                });
            }
        }

        for (auto& t : threads) {
            t.join();
        }

        double threaded_time = timer_threaded.elapsed_ms();
        std::cout << std::setw(10) << k << std::setw(15) << num_threads << std::setw(15) << threaded_time << std::endl;
    }

    return 0;
}