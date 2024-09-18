#include <stdio.h>
#include <stdlib.h>

// 自定义断言宏
#define MY_ASSERT(expr)                                                    \
    ((expr) ? (void)0 :                                                    \
     my_assert_fail(#expr, __FILE__, __LINE__))

//#x 是 C/C++ 预处理器中的一种操作，称为字符串化操作符。它在预处理阶段被处理，用来将宏参数转换为字符串。
//__FILE__ 和 __LINE__ 是 C/C++ 预处理器提供的内置宏，它们可以在编译时自动获取当前代码文件的文件名和行号。

// 断言失败时调用的函数
void my_assert_fail(const char *expr_str, const char *file, int line) {
    printf("Assertion failed: \"%s\", file %s, line %d\n", expr_str, file, line);
    exit(EXIT_FAILURE);  // 退出程序
}

int main() {
    int a = 5;
    int b = 10;

    // 正常情况，断言成功，不会输出错误信息
    MY_ASSERT(a == 5);

    // 错误情况，断言失败，输出文件名、行号和表达式信息
    MY_ASSERT(a == b);  // 这里断言失败，程序会退出

    printf("This line will not be printed if the assert fails.\n");

    return 0;
}
