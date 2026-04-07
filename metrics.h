#pragma once
#include <atomic>

class Metrics {
public:
    std::atomic<int> tasks_completed{0};

    void record() {
        tasks_completed++;
    }
};