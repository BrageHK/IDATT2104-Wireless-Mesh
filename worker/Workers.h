//
// Created by Brage on 15. mai 2023.
//

#ifndef WORKERS_H
#define WORKERS_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class Workers {
private:
    std::mutex wait_mutex;
    std::condition_variable condition;
    bool end = false;
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> threads;

public:
    int num_threads;

    Workers(int num_threads);

    void start();

    void stop();

    void post(std::function<void()> task);

    void join();
};

#endif // WORKERS_H
