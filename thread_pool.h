#pragma once
#include <vector>
#include <thread>
#include <memory>
#include "task.h"
#include "priority_queue.h"
#include "async_logger.h"
#include "metrics.h"

class ThreadPool {
private:
    std::vector<std::thread> workers;
    PriorityQueue<std::unique_ptr<Task>> queue;
    AsyncLogger logger;
    Metrics metrics;

public:
    ThreadPool(size_t threads, size_t queue_capacity = 100);
    ~ThreadPool();

    void submit(std::unique_ptr<Task> task);

    int get_completed_tasks() const;
};