#include "http_server.h"

void handle_fun1(const std::string& url, int client_fd, OnRspCallback rsp_callback) 
{
    rsp_callback(client_fd, "Response from fun1");
}

void handle_fun2(const std::string& url, int client_fd, OnRspCallback rsp_callback) 
{
    rsp_callback(client_fd, "Response from fun2");
}

int main() {
    HttpServer server(9006);
    server.addHandler("/api/fun1", handle_fun1);
    server.addHandler("/api/fun2", handle_fun2);
    server.start();

    return 0;
}
