#include <bits/stdc++.h>

struct rec1 {
    char   *a;
    short   b;
    double  c;
    char    d;
    float   e;
    char    f;
    long    g;
    int     h;
};
/*
rec1
Size: 56
Offsets, Sizes and Padding:
Offset of a: 0, Size: 8, Padding: 0
Offset of b: 8, Size: 2, Padding: 0
Offset of c: 16, Size: 8, Padding: 6
Offset of d: 24, Size: 1, Padding: 0
Offset of e: 28, Size: 4, Padding: 3
Offset of f: 32, Size: 1, Padding: 0
Offset of g: 40, Size: 8, Padding: 7
Offset of h: 48, Size: 4, Padding: 0
*/

// 早年间的一些 CPU 是不支持没有对齐的内存访问的，但是在现代 CPU 上已经没有了这个问题，编译器也提供了一些选项可以让我们明确指出不需要内存对齐。
struct __attribute__((packed)) rec2{
    // 去掉所有对齐的优化
    // 让类的空间密集排布一定会影响内存的读取效率
    char   *a;
    short   b;
    double  c;
    char    d;
    float   e;
    char    f;
    long    g;
    int     h;
};
/*
rec2
Size: 36
Offsets, Sizes and Padding:
Offset of a: 0, Size: 8, Padding: 0
Offset of b: 8, Size: 2, Padding: 0
Offset of c: 10, Size: 8, Padding: 0
Offset of d: 18, Size: 1, Padding: 0
Offset of e: 19, Size: 4, Padding: 0
Offset of f: 23, Size: 1, Padding: 0
Offset of g: 24, Size: 8, Padding: 0
Offset of h: 32, Size: 4, Padding: 0
*/

struct rec3 {
    char   *a;
    double  c;
    long    g;
    float   e;
    int     h;
    short   b;
    char    d;
    char    f;
};
/*
rec3
Size: 40
Offsets, Sizes and Padding:
Offset of a: 0, Size: 8, Padding: 0
Offset of c: 8, Size: 8, Padding: 0
Offset of g: 16, Size: 8, Padding: 0
Offset of e: 24, Size: 4, Padding: 0
Offset of h: 28, Size: 4, Padding: 0
Offset of b: 32, Size: 2, Padding: 0
Offset of d: 34, Size: 1, Padding: 0
Offset of f: 35, Size: 1, Padding: 0
*/

struct __attribute__((aligned(4))) rec4 {
    char   *a;
    short   b;
    double  c;
    char    d;
    float   e;
    char    f;
    long    g;
    int     h;
};

struct __attribute__((aligned(8))) rec8 {
    char   *a;
    short   b;
    double  c;
    char    d;
    float   e;
    char    f;
    long    g;
    int     h;
};

struct __attribute__((aligned(16))) rec16 {
    char   *a;
    short   b;
    double  c;
    char    d;
    float   e;
    char    f;
    long    g;
    int     h;
};

template <typename T>
void printOffsetsAndSizes() {
    std::cout << "Size: " << sizeof(T) << std::endl;

    struct MemberInfo {
        const char* name;
        size_t offset;
        size_t size;
    };

    // C++ 并没有内置的方法来反射或自动获取结构体的成员。所以手动定义成员信息
    std::vector<MemberInfo> members = 
    {
        {"a", offsetof(T, a), sizeof(((T*)nullptr)->a)},
        {"b", offsetof(T, b), sizeof(((T*)nullptr)->b)},
        {"c", offsetof(T, c), sizeof(((T*)nullptr)->c)},
        {"d", offsetof(T, d), sizeof(((T*)nullptr)->d)},
        {"e", offsetof(T, e), sizeof(((T*)nullptr)->e)},
        {"f", offsetof(T, f), sizeof(((T*)nullptr)->f)},
        {"g", offsetof(T, g), sizeof(((T*)nullptr)->g)},
        {"h", offsetof(T, h), sizeof(((T*)nullptr)->h)}
    };

    // 按偏移排序一下
    std::sort(members.begin(),members.end(),[&](MemberInfo& a,MemberInfo& b){return a.offset<b.offset;});

    std::cout << "Offsets, Sizes and Padding:" << std::endl;
    for (size_t i = 0; i < members.size(); ++i) 
    {
        std::cout << "Offset of " << members[i].name << ": " << members[i].offset
                  << ", Size: " << members[i].size 
                  << ", Padding: " << (i+1 < members.size() ? members[i+1].offset : sizeof(T)) - (members[i].offset + members[i].size) << std::endl;
    }
}

int main() {
    std::cout << "rec1" << std::endl;
    printOffsetsAndSizes<rec1>();
    std::cout << "=================================" << std::endl;

    std::cout << "rec2" << std::endl;
    printOffsetsAndSizes<rec2>();
    std::cout << "=================================" << std::endl;

    std::cout << "rec3" << std::endl;
    printOffsetsAndSizes<rec3>();
    std::cout << "=================================" << std::endl;

    std::cout << "rec4" << std::endl;
    printOffsetsAndSizes<rec4>();
    std::cout << "=================================" << std::endl;

    std::cout << "rec8" << std::endl;
    printOffsetsAndSizes<rec8>();
    std::cout << "=================================" << std::endl;

    std::cout << "rec16" << std::endl;
    printOffsetsAndSizes<rec16>();
    std::cout << "=================================" << std::endl;

    return 0;
}
