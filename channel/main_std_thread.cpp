#define NOMINMAX
#include <thread>
#include <numeric>
#include "common.h"
#include "buffered_channel.h"

const int N = 60;
const int NUM_WORKERS = std::thread::hardware_concurrency();

void worker_function(BufferedChannel<BlockTask>& input_channel,
                     BufferedChannel<BlockResult>& output_channel,
                     const Matrix& A, const Matrix& B, int N, int k) {
    
    while (true) {
        std::pair<BlockTask, bool> task_pair;
        try {
            task_pair = input_channel.Recv();
        } catch (const std::runtime_error& e) {
            break;
        }

        if (!task_pair.second) {
            break; 
        }

        BlockTask task = std::move(task_pair.first);
        
        BlockResult result = compute_result_block_and_return(A, B, N, k, 
                                                             task.i_block, task.j_block);
        
        try {
            output_channel.Send(std::move(result));
        } catch (const std::runtime_error& e) {
            std::cerr << "Error sending result: " << e.what() << std::endl;
            break;
        }
    }
}

void channel_multiply(const Matrix& A, const Matrix& B, Matrix& C, int N, int k) {
    
    int num_blocks_dim = (N + k - 1) / k;
    int total_tasks = num_blocks_dim * num_blocks_dim;
    BufferedChannel<BlockTask> input_channel(total_tasks); 
    BufferedChannel<BlockResult> output_channel(total_tasks); 

    std::vector<std::thread> workers;

    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers.emplace_back(worker_function, std::ref(input_channel), std::ref(output_channel),
                             std::ref(A), std::ref(B), N, k);
    }

    for (int i_block = 0; i_block < num_blocks_dim; ++i_block) {
        for (int j_block = 0; j_block < num_blocks_dim; ++j_block) {
            input_channel.Send({i_block, j_block});
        }
    }

    input_channel.Close(); 

    int results_count = 0;
    while (results_count < total_tasks) {
        std::pair<BlockResult, bool> result_pair;
        try {
            result_pair = output_channel.Recv();
        } catch (const std::runtime_error& e) {
            break;
        }

        if (!result_pair.second) {
            break;
        }

        BlockResult result = std::move(result_pair.first);
        
        for (int i = result.row_start; i < result.row_end; ++i) {
            for (int j = result.col_start; j < result.col_end; ++j) {
                C[i][j] = result.block_data[i - result.row_start][j - result.col_start];
            }
        }
        results_count++;
    }

    output_channel.Close(); 

    for (auto& w : workers) {
        w.join();
    }
}


int main() {
    Matrix A = create_matrix(N);
    Matrix B = create_matrix(N);
    Matrix C_seq = create_matrix(N);
    Matrix C_channel = create_matrix(N);

    fill_matrix(A, N);
    fill_matrix(B, N);

    std::cout << "Matrix Multiplication Benchmark (N = " << N << ")" << std::endl;
    std::cout << "Using BufferedChannel with " << NUM_WORKERS << " workers\n" << std::endl;

    Timer timer_seq;
    sequential_multiply(A, B, C_seq, N);
    double seq_time = timer_seq.elapsed_ms();
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Sequential (1 Thread) Time: " << seq_time << " ms" << std::endl;
    std::cout << "------------------------------------------------\n" << std::endl;
    
    std::cout << std::setw(10) << "Block K" << std::setw(15) << "Num Workers" << std::setw(15) << "Time (ms)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int k = 1; k <= N; ++k) {
        
        C_channel = create_matrix(N);

        Timer timer_channel;
        channel_multiply(A, B, C_channel, N, k);
        double channel_time = timer_channel.elapsed_ms();
        
        std::cout << std::setw(10) << k << std::setw(15) << NUM_WORKERS << std::setw(15) << channel_time << std::endl;
    }

    return 0;
}