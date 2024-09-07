#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    // 创建 epoll 实例，返回 epoll 文件描述符
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "epoll_create1 错误。\n";
        return 1;
    }

    struct epoll_event ev;        // 定义 epoll_event 结构体
    ev.events = EPOLLIN;          // 设置监听的事件为“可读”
    ev.data.fd = STDIN_FILENO;    // 监听标准输入

    // 将标准输入文件描述符加入 epoll 实例中
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        std::cerr << "epoll_ctl 错误。\n";
        return 1;
    }

    struct epoll_event events[10];   // 存储事件的数组
    std::cout << "等待输入(5秒超时)...\n";
    // 调用 epoll_wait，等待事件发生
    int result = epoll_wait(epoll_fd, events, 10, 5000);

    if (result > 0) {
        // 如果有事件发生，遍历所有就绪的文件描述符
        for (int i = 0; i < result; ++i) {
            if (events[i].events & EPOLLIN) {
                char buffer[100];
                // 读取输入
                read(STDIN_FILENO, buffer, sizeof(buffer));
                std::cout << "输入内容: " << buffer << std::endl;
            }
        }
    } else if (result == 0) {
        // 超时
        std::cout << "超时，没有输入。\n";
    } else {
        // 出错
        std::cerr << "epoll_wait 错误。\n";
    }

    // 关闭 epoll 文件描述符
    close(epoll_fd);
    return 0;
}
