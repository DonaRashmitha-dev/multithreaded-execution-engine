#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class PriorityQueue {
private:
    std::priority_queue<T> pq;
    std::mutex mtx;
    std::condition_variable cv;

    size_t capacity;
    bool shutdown = false;

public:
    PriorityQueue(size_t cap) : capacity(cap) {}

    void push(T item) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [&]() {
            return pq.size() < capacity || shutdown;
        });

        if (shutdown) return;

        pq.push(std::move(item));
        cv.notify_all();
    }

    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [&]() {
            return !pq.empty() || shutdown;
        });

        if (shutdown && pq.empty()) return false;

        item = std::move(const_cast<T&>(pq.top()));
        pq.pop();

        cv.notify_all();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            shutdown = true;
        }
        cv.notify_all();
    }
};