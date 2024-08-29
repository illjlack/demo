#include "http_server.h"

bool HttpServer::start()
{
    // 创建一个IPv4 TCP套接字
    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listen_fd < 0)
    {
        return false;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;           // 设置为IPv4地址
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 监听所有可用的网络接口
    server_addr.sin_port = htons(m_port);       // 将端口号转换为网络字节序

    // 将套接字绑定到指定的IP地址和端口号
    if (bind(m_listen_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        return false;
    }
    // 开始监听传入的连接，最大等待队列为128
    if (listen(m_listen_fd, 128) < 0)
    {
        return false;
    }

    std::cout << "Server started on port " << m_port << std::endl;

    // 无限循环，处理客户端的连接
    while (true) 
    {
        // 接受客户端连接请求
        int client_fd = accept(m_listen_fd, nullptr, nullptr);
        if (client_fd >= 0) handleClient(client_fd); // 处理客户端请求
    }
    return true;
}

// 添加URL对应的处理函数
void HttpServer::addHandler(const std::string &url, ReqHandler handler)
{
    m_handlers[url] = handler; // 将处理函数与URL对应保存到哈希表中
}

// 处理客户端请求
void HttpServer::handleClient(int client_fd)
{
    char buffer[1024];
    int len = recv(client_fd, buffer, sizeof(buffer) - 1, 0); // 接收客户端发送的数据
    if (len <= 0) 
    { 
        close(client_fd); // 如果接收失败或客户端关闭连接，则关闭客户端连接
        return;
    }
    buffer[len] = '\0'; // 将数据末尾设置为字符串终止符

    std::string request(buffer);
    // 提取URL
    size_t url_start = request.find(' ') + 1;
    size_t url_end = request.find(' ', url_start);
    std::string url = request.substr(url_start, url_end - url_start);

    // 查找是否有对应URL的处理函数
    auto it = m_handlers.find(url);
    if (it != m_handlers.end()) 
    {
        // 如果找到处理函数，调用它并发送响应
        it->second(url, client_fd, 
            [](int fd, const std::string& response)
            {
                std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response.size()) + "\r\n\r\n" + response;
                send(fd, http_response.c_str(), http_response.size(), 0);
                close(fd); // 发送完响应后关闭连接
            });
    } 
    else 
    {
        // 如果没有对应的处理函数，返回404 Not Found响应
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        close(client_fd); // 发送完响应后关闭连接
    }
}

// 关闭服务器并释放资源
void HttpServer::closeServer()
{
    if (m_listen_fd >= 0) 
    {
        close(m_listen_fd); // 关闭监听套接字
        m_listen_fd = -1; // 重置文件描述符
    }
}
