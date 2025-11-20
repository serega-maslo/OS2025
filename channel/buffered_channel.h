#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <deque>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <utility>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size)
        : buffer_size_(size), is_closed_(false) {
        if (size <= 0) {
            throw std::invalid_argument("Buffer size must be positive.");
        }
    }

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (is_closed_) {
            throw std::runtime_error("Attempt to send on a closed channel.");
        }

        send_cv_.wait(lock, [this] { 
            return is_closed_ || (buffer_.size() < buffer_size_); 
        });

        if (is_closed_) {
            throw std::runtime_error("Attempt to send on a closed channel.");
        }

        buffer_.push_back(std::move(value));

        recv_cv_.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);

        recv_cv_.wait(lock, [this] { 
            return is_closed_ || !buffer_.empty(); 
        });

        if (is_closed_ && buffer_.empty()) {
            return {T(), false}; 
        }

        T value = std::move(buffer_.front());
        buffer_.pop_front();

        send_cv_.notify_one();

        return {std::move(value), true};
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (is_closed_) {
            return;
        }
        is_closed_ = true;

        send_cv_.notify_all();
        recv_cv_.notify_all();
    }

private:
    std::deque<T> buffer_;
    const int buffer_size_;
    bool is_closed_;

    std::mutex mutex_;
    std::condition_variable send_cv_;
    std::condition_variable recv_cv_;
};

#endif // BUFFERED_CHANNEL_H_