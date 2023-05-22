//
// Created by Brage on 15. mai 2023.
//

#include "Workers.h"
#include <cmath>


Workers::Workers(int num_threads) : num_threads(num_threads) {}

void Workers::start() {
    for(int i = 0; i < num_threads; i++) {
        threads.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->wait_mutex);
                    this->condition.wait(lock, [this] {
                        return this->end || !this->tasks.empty();
                    });
                    if (this->end && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

void Workers::stop() {
    {
        std::unique_lock<std::mutex> lock(wait_mutex);
        end = true;
    }
    condition.notify_all();
    join();
}

void Workers::post(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(wait_mutex);
        tasks.emplace(task);
    }
    condition.notify_one();
}

void Workers::join() {
    for(std::thread &thread : threads)
        thread.join();
}
