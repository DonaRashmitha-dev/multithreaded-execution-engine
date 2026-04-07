#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

class AsyncLogger {
private:
    std::queue<std::string> logs;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop{false};
    std::thread worker;

    const size_t MAX_LOGS = 1000;

public:
    AsyncLogger() {
        worker = std::thread([this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mtx);

                cv.wait(lock, [&]() {
                    return !logs.empty() || stop;
                });

                while (!logs.empty()) {
                    std::cout << logs.front() << std::endl;
                    logs.pop();
                }

                if (stop && logs.empty()) break;
            }
        });
    }

    void log(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mtx);

        if (logs.size() < MAX_LOGS) {
            logs.push(msg);
            cv.notify_one();
        }
        // else drop logs (backpressure)
    }

    ~AsyncLogger() {
        stop = true;
        cv.notify_all();
        if (worker.joinable()) worker.join();
    }
};