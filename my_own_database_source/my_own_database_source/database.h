#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#ifdef _WIN64
typedef long long ssize_t;
#else
typedef int ssize_t;
#endif
#else
#include <sys/types.h>
#endif

// 输入缓冲区类型
typedef struct {
	char* buffer;			// 缓冲区
	size_t buffer_length;	// 缓冲区长度
	ssize_t input_length;	// 输入长度
} InputBuffer;

// 开辟新的输入缓冲区
InputBuffer* new_input_buffer();
// 打印提示词
void print_prompt();
// 读取输入缓冲区
void read_input(InputBuffer* input_buffer);
// 关闭输入缓冲区
void close_input_buffer(InputBuffer* input_buffer);
// 测试 读取-求值（执行）-输出循环
void test_REPL();