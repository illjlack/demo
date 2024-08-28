#include <iostream>
#include <vector>
#include <chrono>
#include "ThreadPool.h"

int main() {
    // 创建一个包含4个线程的线程池
    ThreadPool pool(4);

    // 存储每个任务的结果
    std::vector<std::future<int>> results;

    // 向线程池添加8个任务
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
                pool.enqueue([i] {
                    std::cout << "Task " << i << " started" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << "Task " << i << " finished" << std::endl;
                    return i * i;  // 返回 i 的平方
                })
        );
    }

    // 获取并打印每个任务的结果
    for(auto && result: results) {
        // result.get() 是阻塞的
        std::cout << "Result: " << result.get() << std::endl;
    }

    return 0;
}
