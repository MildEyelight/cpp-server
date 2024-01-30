#pragma once
#include <thread>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <future>
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

    template<typename F, typename... Args>
    auto add_task(F&& func, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using reture_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<reture_type()>>(
            std::bind(std::forward<F>(func), std::forward<Args...>(args)...)
        );

        std::future<reture_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.emplace([task](){(*task)();});
        }
        cv.notify_one();
        return res;
    }
    int add_task(std::function<void()> task);
};