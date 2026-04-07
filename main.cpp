#include "thread_pool.h"
#include <chrono>
#include <iostream>

int main() {
    ThreadPool pool(4, 50);

    for (int i = 0; i < 20; i++) {
        pool.submit(std::make_unique<Task>(i, i % 3, [i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Task " << i << " completed\n";
        }));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Tasks completed: "
              << pool.get_completed_tasks()
              << std::endl;

    return 0;
}