#include <iostream>
#include <poll.h>
#include <unistd.h>

int main() {
    struct pollfd fds;              // 定义 pollfd 结构体
    fds.fd = STDIN_FILENO;          // 设置监听的文件描述符为标准输入
    fds.events = POLLIN;            // 设置监听的事件为“可读”

    int timeout = 5000;             // 超时设置为5秒（单位：毫秒）
    std::cout << "等待输入(5秒超时)...\n";

    // 调用 poll 函数，监听文件描述符
    // 参数1：&fds，文件描述符数组
    // 参数2：1，文件描述符数组的大小（监听一个文件描述符）
    // 参数3：timeout，超时时间，单位毫秒
    int result = poll(&fds, 1, timeout);

    if (result > 0) {
        // 如果有事件发生，检查是否是可读事件
        if (fds.revents & POLLIN) {
            char buffer[100];
            // 读取输入
            read(STDIN_FILENO, buffer, sizeof(buffer));
            std::cout << "输入内容: " << buffer << std::endl;
        }
    } else if (result == 0) {
        // 超时
        std::cout << "超时，没有输入。\n";
    } else {
        // 出错
        std::cerr << "poll 错误。\n";
    }

    return 0;
}
