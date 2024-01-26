#include "ThreadPool.h"

ThreadPool::ThreadPool(int thread_num):stop(false){
    for(int i = 0; i < thread_num; i ++){
        thread_pool.emplace_back(std::thread(this->worker));
    }
}

//TODO : safty exit
ThreadPool::~ThreadPool(){
    stop = true;
    cv.notify_all();
}

int ThreadPool::add_task(std::function<void()> task){
    {
        std::unique_lock<std::mutex> lock(this->mtx);
        task_queue.push(std::move(task));
    }
    cv.notify_one();
    return 0;
}