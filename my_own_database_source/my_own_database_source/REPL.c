#define _CRT_SECURE_NO_WARNINGS 1
#include "database.h"

// 2026.01.12记录REPL的C语言实现源代码

// 读取——求值——输出 循环流程
// read   execute print loop
// 通过输入缓冲区指针接收输入缓冲区的内容
// 判断输入缓冲区的内容
// 
// 读取操作
// 为输入缓冲区指针开辟相应空间
// 通过 getline() 读取输入缓冲区中的一行内容
// ssize_t getline(char **restrict lineptr, size_t *restrict n, FILE* restrict stream); —— Linux 系统下的文件函数
// char * fgets ( char * str, int num, FILE * stream );
// 求值操作
// 获取当前缓冲区指针的具体值
// 输出操作
// 通过读取到的具体信息给出相应的输出



// 开辟新的输入缓冲区
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}
// 打印提示词
void print_prompt() { printf("db > "); }
// 读取输入缓冲区
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}
// 关闭输入缓冲区
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
// 测试 读取-求值（执行）-输出循环
void test_REPL() {
    InputBuffer* input_buffer = new_input_buffer();
    // 循环
    while (true) {
        // 输出
        print_prompt();
        // 读取
        read_input(input_buffer);
        // 处理
        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
    }
}