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

// -----------------------------------
// REPL 使用结构体声明
// 输入缓冲区类型
typedef struct {
	char* buffer;			// 缓冲区
	size_t buffer_length;	// 缓冲区长度
	ssize_t input_length;	// 输入长度
} InputBuffer;


// --------------------------------
// SQL_VM 使用结构体声明
//@@ - 10, 6 + 10, 23 @@ struct InputBuffer_t {
//} InputBuffer;

// @@ - 10, 6 + 10, 23 @@ —— 版本控制信息
// @@ …… @@ —— 差异化头部
// - —— 原版本
// 10 —— 第十行
// 6 —— 改动6行
// + —— 现版本
// 10 —— 第十行
// 23 —— 改动23行

typedef enum {
    META_COMMAND_SUCCESS,						// meta 命令成功
    META_COMMAND_UNRECOGNIZED_COMMAND			// meta 命令无法识别
} MetaCommandResult;							// meta 命令结果

typedef enum { 
	PREPARE_SUCCESS,							// 准备成功
	PREPARE_UNRECOGNIZED_STATEMENT				// 准备无法识别状态
} PrepareResult;								// 准备结果

typedef enum { 
	STATEMENT_INSERT,							// 插入状态
	STATEMENT_SELECT							// 选择状态
} StatementType;								// 状态类型

typedef struct {
	StatementType type;							// 状态类型变量 
} Statement;									// 状态

// ----------------------------------
// REPL 使用函数声明

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

// ----------------------------------------
// SQL_VM 使用函数声明

