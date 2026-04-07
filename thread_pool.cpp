#include "thread_pool.h"
#include <sstream>

ThreadPool::ThreadPool(size_t threads, size_t queue_capacity)
    : queue(queue_capacity) {

    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this, i]() {
            while (true) {
                std::unique_ptr<Task> task;

                if (!queue.pop(task)) break;

                try {
                    std::stringstream ss;
                    ss << "Thread " << i << " executing Task " << task->id;
                    logger.log(ss.str());

                    if (task->func) {
                        task->func();
                    }

                    metrics.record();

                } catch (...) {
                    logger.log("Task crashed");
                }
            }
        });
    }
}

void ThreadPool::submit(std::unique_ptr<Task> task) {
    queue.push(std::move(task));
}

int ThreadPool::get_completed_tasks() const {
    return metrics.tasks_completed.load();
}

ThreadPool::~ThreadPool() {
    queue.stop();

    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }
}