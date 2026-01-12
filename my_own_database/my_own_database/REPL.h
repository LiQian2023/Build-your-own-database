#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#ifdef _WIN32
#ifdef _WIN64
typedef long long ssize_t;
#else
typedef int ssize_t;
#endif
#else
#include <sys/types.h>
#endif

typedef struct Input_Buffer {
	char* buffer;				// 字符串指针
	size_t buffer_length;		// 缓冲区长度
	ssize_t input_length;		// 输入长度
}Input_Buffer;

// 初始化输入缓冲区指针
Input_Buffer* new_input_buffer();
// PRINT
Print_promp();
// READ
void Read_input(Input_Buffer** input_buffer);
// 获取当前缓冲区一行长度
ssize_t my_getline(char** buffer, size_t* buffer_length, FILE* stream);
// 关闭输入缓冲区
void close_input_buffer(Input_Buffer** input_buffer);
// 打印输入缓冲区
void Print_read(Input_Buffer** input_buffer);
// REPL测试
void test_REPL();
