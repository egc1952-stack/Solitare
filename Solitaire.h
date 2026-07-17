// ThreadPool.h
#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>

int initialize();
int makeNewDeal();


class ThreadPool {
public:
    explicit ThreadPool(std::size_t threads = std::thread::hardware_concurrency())
        : stop_(false)
    {
        if (threads == 0) threads = 1;

        for (std::size_t i = 0; i < threads; ++i) {
            workers_.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        cv_.wait(lock, [this] {
                            return stop_ || !tasks_.empty();
                        });

                        if (stop_ && tasks_.empty())
                            return;

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto &t : workers_) {
            if (t.joinable()) t.join();
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using Ret = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<Ret()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<Ret> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stop_)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks_.emplace([task] { (*task)(); });
        }
        cv_.notify_one();
        return res;
    }

private:
    std::vector<std::thread>          workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex                        mutex_;
    std::condition_variable           cv_;
    std::atomic<bool>                 stop_;
};
