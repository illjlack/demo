#include <iostream>
#include <chrono>
#include <cstdlib> // 用于 system() 函数
#include <vector>
#include <numeric>
#include <string>

int main(int argc, char* argv[]) {
    // 检查是否提供了可执行文件路径
    if (argc < 2) {
        std::cerr << "用法: " << argv[0] << " <可执行文件路径> [测试次数]" << std::endl;
        return 1;
    }

    // 获取可执行文件的路径
    std::string executable_path = argv[1];
    
    // 默认测试次数为10次
    int num_iterations = 10;

    // 如果提供了测试次数参数，则解析该参数
    if (argc >= 3) {
        num_iterations = std::stoi(argv[2]);
    }

    // 用于存储每次执行的时间
    std::vector<long long> durations;

    // 多次运行可执行文件
    for (int i = 0; i < num_iterations; ++i) {
        std::cout << "正在执行第 " << (i + 1) << " 次测试..." << std::endl;

        // 记录开始时间
        auto start_time = std::chrono::high_resolution_clock::now();

        // 执行外部可执行文件
        int ret = std::system(executable_path.c_str());

        // 检查可执行文件的返回码，如果失败则退出程序
        if (ret != 0) {
            std::cerr << "错误: 可执行文件 " << executable_path << " 返回了非零的退出码。" << std::endl;
            return ret;
        }

        // 记录结束时间
        auto end_time = std::chrono::high_resolution_clock::now();
        
        // 计算执行时间并转换为毫秒
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        // 存储该次执行的时间
        durations.push_back(duration);
        
        std::cout << "第 " << (i + 1) << " 次执行时间: " << duration << " 毫秒" << std::endl;
    }

    // 计算总时间
    long long total_duration = std::accumulate(durations.begin(), durations.end(), 0LL);
    
    // 计算平均执行时间
    double average_duration = static_cast<double>(total_duration) / num_iterations;

    // 输出平均执行时间
    std::cout << "经过 " << num_iterations << " 次测试后的平均执行时间: " 
              << average_duration << " 毫秒" << std::endl;

    return 0;
}
