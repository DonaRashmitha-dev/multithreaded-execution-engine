#pragma once
#include <functional>

class Task {
public:
    int id;
    int priority;
    int age; // for starvation mitigation
    std::function<void()> func;

    Task(int id, int priority, std::function<void()> f)
        : id(id), priority(priority), age(0), func(f) {}

    // Higher effective priority = priority + age
    bool operator<(const Task& other) const {
        return (priority + age) < (other.priority + other.age);
    }
};