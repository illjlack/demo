#include <iostream>
#include <sys/select.h>
#include <unistd.h>

int main() {
    fd_set readfds;            // 定义文件描述符集合
    FD_ZERO(&readfds);         // 清空集合
    
    int fd = STDIN_FILENO;     // 获取标准输入的文件描述符
    FD_SET(fd, &readfds);      // 将标准输入文件描述符加入集合

    // 设置超时时间
    struct timeval timeout;
    timeout.tv_sec = 5;        // 设置超时时间为5秒
    timeout.tv_usec = 0;       // 微秒部分为0

    std::cout << "等待输入(5秒超时)...\n";

    // select函数监听多个文件描述符
    // 参数1：fd + 1，监听的最大文件描述符值加1 , 会监听fd从0~fd的所有被readfds设置的文件
    // 参数2：&readfds，监听可读事件的文件描述符集合
    // select 通过一个位图（bitmask）来表示文件描述符集。当你调用 FD_SET() 函数时，select 将在内部的位图中设置对应文件描述符的位置为1
    // fd_set 最大1024 常见的 UNIX 系统默认限制为一个进程最多可以打开 1024 个文件描述符（可通过修改系统参数提高此限制）
    // 参数3：nullptr，不监听可写事件，传入空指针
    // 参数4：nullptr，不监听异常事件，传入空指针
    // 参数5：&timeout，超时时间为5秒
    int result = select(fd + 1, &readfds, nullptr, nullptr, &timeout);
    
    if (result > 0) {
        // 如果有文件描述符就绪
        if (FD_ISSET(fd, &readfds)) {
            char buffer[100];
            // 读取输入
            read(fd, buffer, sizeof(buffer));
            std::cout << "输入内容: " << buffer << std::endl;
        }
    } else if (result == 0) {
        // 超时
        std::cout << "超时，没有输入。\n";
    } else {
        // 出错
        std::cerr << "select 错误。\n";
    }

    return 0;
}
