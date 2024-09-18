#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <emmintrin.h>

// 自旋锁类
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

class ReadWriteLock {
public:
    // 获取读锁
    void read_lock() {
        while (write_request_flag_.load()) {
            // 如果有写者等待，读者暂时让出CPU
            std::this_thread::yield();
        }
        if (reader_count_.fetch_add(1) == 0) {
            writer_lock_.lock();  // 如果是第一个读者，则获取写锁
        }
    }

    // 释放读锁
    void read_unlock() {
        if (reader_count_.fetch_sub(1) == 1) {
            writer_lock_.unlock();  // 如果是最后一个读者，则释放写锁
        }
    }

    // 获取写锁
    void write_lock() {
        write_request_flag_.store(true);  // 标记写者请求
        writer_lock_.lock();
    }

    // 释放写锁
    void write_unlock() {
        write_request_flag_.store(false);  // 清除写者请求标记
        writer_lock_.unlock();
    }

private:
    Spinlock writer_lock_; // 用于控制写操作的访问
    std::atomic<int> reader_count_{0}; // 当前活动读者的数量
    std::atomic<bool> write_request_flag_{false}; // 用于标记是否有写者在等待
};


int main() {
    ReadWriteLock rwlock;

    // 示例使用
    std::thread reader1([&rwlock]() {
        rwlock.read_lock();
        std::cout<<"reader1"<<std::endl;
        rwlock.read_unlock();
    });

    std::thread writer([&rwlock]() {
        rwlock.write_lock();
        std::cout<<"writer"<<std::endl;
        rwlock.write_unlock();
    });

    std::thread reader2([&rwlock]() {
        rwlock.read_lock();
        std::cout<<"reader2"<<std::endl;
        rwlock.read_unlock();
    });

    reader1.join();
    writer.join();
    reader2.join();

    return 0;
}
