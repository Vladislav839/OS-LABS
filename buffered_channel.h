#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <thread>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <exception>

template<class T>
class BufferedChannel {
private:
    int size;
    std::queue<T> buffer_queue;
    std::mutex mt;
    std::condition_variable condition;
    bool open = true;

public:
    explicit BufferedChannel(int size) : size(size) {}

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mt);
        condition.wait(lock, [this] { return buffer_queue.size() != size || !open; });
        if (!open) throw(std::runtime_error("Can't send data to closed channel"));
        buffer_queue.push(value);
        lock.unlock();
        condition.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mt);
        condition.wait(lock, [this] { return !buffer_queue.empty() || !open; });
        T value = buffer_queue.empty() ? T() : buffer_queue.front();
        if(!buffer_queue.empty())  buffer_queue.pop();
        lock.unlock();
        condition.notify_one();
        return buffer_queue.empty() ? std::make_pair(T(), false) :
            std::make_pair(value, true);
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mt);
        open = false;
        lock.unlock();
        condition.notify_all();
    }

    int Size() {
        std::lock_guard<std::mutex> lock(mt);
        return buffer_queue.size(); 
    }

private:
};

#endif