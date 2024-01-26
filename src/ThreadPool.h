#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
class ThreadPool{
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::function<void()>> task_queue;
    std::vector<std::thread> thread_pool;
    bool stop;
private:
    std::function<void()> worker = [this](){
        while(true){
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){
                    return stop || !task_queue.empty();
                });
                if(stop && task_queue.empty()) return;
                task = task_queue.front();
                task_queue.pop();
            }
            task();
        }
    };
public:
    ThreadPool(int thread_num = 8);
    ~ThreadPool();
    int add_task(std::function<void()> task);
};