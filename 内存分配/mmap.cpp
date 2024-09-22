#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // 分配一块匿名内存
    void* addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    printf("Allocated memory at: %p\n", addr);

    // 写入数据
    sprintf((char*)addr, "Hello, mmap!");

    // 读取数据
    printf("Data: %s\n", (char*)addr);

    // 释放内存
    if (munmap(addr, 4096) == -1) {
        perror("munmap failed");
        return 1;
    }

    return 0;
}