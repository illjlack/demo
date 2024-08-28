#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace std{
    template<typename F, typename... Args>
    struct invoke_result {
        using type = decltype(std::declval<F>()(std::declval<Args>()...));
    };
}


// 线程池类的定义
class ThreadPool {
public:
    // 构造函数：初始化线程池并启动指定数量的线程
    explicit ThreadPool(size_t threads);
    // 析构函数：停止线程池并等待所有线程完成
    ~ThreadPool();

    // 添加新任务到线程池中，任务可以是任意的可调用对象
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type>; // 推导返回值类型
    // std::future,表示一个异步操作的结果,可以在任务完成后获取返回值。
    /*
     cpp11、14 用 std::result_of<F(Args...)>::type
     或者自己定义：
     template<typename F, typename... Args>
        struct invoke_result {
        using type = decltype(std::declval<F>()(std::declval<Args>()...));
    };
    */

private:
    std::vector<std::thread> workers;               // 工作线程
    std::queue<std::function<void()>> tasks;        // 任务队列
    std::mutex queue_mutex;                         // 任务队列的互斥锁，保护任务队列的访问
    std::condition_variable condition;              // 条件变量，用于任务队列的同步
    bool stop;                                      // 标志变量，表示线程池是否停止
};

// 构造函数实现：启动指定数量的线程
// inline 防止重复定义，即使这个函数被多个翻译单元包含并定义，只需保留一个定义
inline ThreadPool::ThreadPool(size_t threads)
        : stop(false)
{
    for(size_t i = 0; i < threads; ++i) {
        workers.emplace_back(
                [this] {
                    for(;;)                                                              // （启动工作线程）无限循环，不断从任务队列中取出任务执行
                    {
                        std::function<void()> task;
                        { // 锁的作用域
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock, [this]{                     // 等待条件变量通知，有任务或线程池停止
                                return this->stop || !this->tasks.empty();
                            });
                            if(this->stop && this->tasks.empty())                  // 如果线程池停止并且任务队列为空，则退出循环
                                return;
                            task = std::move(this->tasks.front());                 // 取出任务队列中的第一个任务
                            this->tasks.pop();                                     // 从队列中移除该任务
                        }
                        task(); // 执行任务
                    }
                }
        );
    }
}

// 析构函数实现：停止所有线程并等待它们完成任务
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;  // 设置停止标志
    }
    condition.notify_all();  // 通知所有线程
    for(std::thread &worker: workers)  
        worker.join(); // 等待线程完成后关闭线程
}

// `enqueue` 方法实现：向线程池的任务队列中添加新任务
// 模板函数默认是内联的。
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::invoke_result<F, Args...>::type>
{
    using return_type = typename std::invoke_result<F, Args...>::type;

    // 将任务包装成 `std::packaged_task`，以便获取任务的返回值
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future(); // 获取任务的 future 对象，以便稍后获取任务的结果
    {
        std::unique_lock<std::mutex> lock(queue_mutex); // 加锁访问任务队列
        // 如果线程池已停止，不允许添加新任务
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]{ (*task)(); });  // 将任务加入任务队列
    }
    // 通知一个等待的工作线程
    condition.notify_one();
    // 返回任务的 future 对象
    return res;
}