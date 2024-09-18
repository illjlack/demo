#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <emmintrin.h> // _mm_pause()

class Spinlock {
private:
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT; // 用于表示锁的状态,这个宏其实就是0

public:
    void lock() {
        while (lock_flag.test_and_set(std::memory_order_acquire)) {
            // 自旋等待，CPU空转
            _mm_pause(); // 让出CPU,减少总线争用,（就是执行一个cpu的指令，占一点cpu时间,但是不需要真的计算什么）
        }
    }

    void unlock() {
        lock_flag.clear(std::memory_order_release); // 释放锁
    }
};
// 测试函数，多个线程同时增加共享变量
void spinlock_increment(Spinlock& spinlock, int& counter, int increments) {
    for (int i = 0; i < increments; ++i) {
        spinlock.lock();
        ++counter;
        spinlock.unlock();
    }
}

void test_spinlock() {
    const int num_threads = 8;
    const int increments_per_thread = 100000;

    Spinlock spinlock;
    int counter = 0;

    std::vector<std::thread> threads;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        //使用 std::thread 或其他异步任务时，传递给线程函数的参数默认是通过拷贝传递的,std::ref 可以确保传递引用
        threads.emplace_back(spinlock_increment, std::ref(spinlock), std::ref(counter), increments_per_thread);
    }
    
    for (auto& th : threads) {
        th.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Final counter value (Spinlock): " << counter << "\n";
    std::cout << "Execution time (Spinlock): " << duration.count() << " ms\n";
}


int main()
{
    test_spinlock();
    return 0;
}