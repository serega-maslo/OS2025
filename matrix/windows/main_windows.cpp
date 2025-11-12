#define NOMINMAX
#include <windows.h>
#include "common.h"
const int N = 60;

struct ThreadArgs {
    const Matrix* A;
    const Matrix* B;
    Matrix* C;
    int N, k, i_block, j_block;
};

DWORD WINAPI ThreadEntry(LPVOID lpParam) {
    ThreadArgs* args = static_cast<ThreadArgs*>(lpParam);
    
    compute_result_block(
        *(args->A), *(args->B), *(args->C),
        args->N, args->k, args->i_block, args->j_block
    );
    
    delete args;
    return 0;
}

int main() {
    Matrix A = create_matrix(N);
    Matrix B = create_matrix(N);
    Matrix C_seq = create_matrix(N);
    Matrix C_threaded = create_matrix(N);

    fill_matrix(A, N);
    fill_matrix(B, N);

    std::cout << "Matrix Multiplication Benchmark (N = " << N << ")" << std::endl;
    std::cout << "Using <windows.h>\n" << std::endl;

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

        std::vector<HANDLE> hThreads;
        hThreads.reserve(num_threads);

        Timer timer_threaded;

        for (int i_block = 0; i_block < num_blocks_dim; ++i_block) {
            for (int j_block = 0; j_block < num_blocks_dim; ++j_block) {
                ThreadArgs* args = new ThreadArgs{ &A, &B, &C_threaded, N, k, i_block, j_block };
                
                HANDLE hThread = CreateThread(
                    NULL,
                    0,
                    ThreadEntry,
                    args,
                    0,
                    NULL);

                if (hThread != NULL) {
                    hThreads.push_back(hThread);
                }
            }
        }

        WaitForMultipleObjects(static_cast<DWORD>(hThreads.size()), hThreads.data(), TRUE, INFINITE);

        double threaded_time = timer_threaded.elapsed_ms();

        for (HANDLE h : hThreads) {
            CloseHandle(h);
        }

        std::cout << std::setw(10) << k << std::setw(15) << num_threads << std::setw(15) << threaded_time << std::endl;
    }

    return 0;
}