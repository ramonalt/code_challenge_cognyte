#pragma once

#include <queue>
#include <mutex>

template<typename T>
class atomic_queue {
    private:
        std::mutex lock = std::mutex();
        std::queue<T> inner_queue = {};

    public:
        inline void atomic_push(T& val) {
            lock.lock();
            inner_queue.push(val);
            lock.unlock();
        };

        inline T atomic_front_and_pop() {
            lock.lock();
            T val = inner_queue.front();
            inner_queue.pop();
            lock.unlock();

            return val;
        };

        inline size_t atomic_size() {
            lock.lock();
            size_t val = inner_queue.size();
            lock.unlock();
            
            return val;
        };

};