#include <unistd.h>
#include <stdio.h>

int main() {
    void* heap_end = sbrk(0);  // 获取当前堆末尾地址
    printf("Current heap end: %p\n", heap_end);

    // 扩展堆空间
    if (sbrk(1024) == (void*)-1) {
        perror("sbrk failed");
        return 1;
    }

    heap_end = sbrk(0);  // 获取新的堆末尾地址
    printf("New heap end: %p\n", heap_end);

    return 0;
}