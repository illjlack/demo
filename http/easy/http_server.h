#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

// http回调类型
using OnRspCallback = std::function<void(int, const std::string&)>;
// http请求处理器类型
using ReqHandler = std::function<void(const std::string&, int, OnRspCallback)>;

class HttpServer
{
public:
    HttpServer(int port) : m_port(port) {}
    ~HttpServer() { closeServer(); }

    bool start();
    void addHandler(const std::string &url, ReqHandler handler);

private:
    void handleClient(int client_fd);
    void closeServer();

    int m_port;
    int m_listen_fd = -1;
    std::unordered_map<std::string, ReqHandler> m_handlers;
};
