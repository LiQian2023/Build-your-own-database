#pragma once
// 头文件引用模块
// REPL 模块引用头文件
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
// Hard_Code_Table insert 模块引用头文件
// @@ - 2, 6 + 2, 7 @@
#include <stdint.h>

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
// 结构体、宏定义模块
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
#if 0
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
#endif

// --------------------------------
// Hard_Code_Table insert  模块使用结构体声明
//typedef struct {
//    char* buffer;
//    @@ - 10, 6 + 11, 105 @@ typedef struct {
//    } InputBuffer;

// 执行结果
typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;
// 系统指令结果
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;
// 准备结果
typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;
// 状态类型
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
// 每一行中使用者名字这一列的最大长度
#define COLUMN_USERNAME_SIZE 32
// 每一行中电子邮箱这一列的最大长度
#define COLUMN_EMAIL_SIZE 255
// 行
typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;
// 状态
typedef struct {
    StatementType type;
    Row row_to_insert; //only used by insert statement

} Statement;
// 计算结构体成员大小的宏
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#if 0
// GCC下实现方式
// const uint32_t ID_SIZE = size_of_attribute(Row, id);
// const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
// const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
// const uint32_t ID_OFFSET = 0;
// const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
// const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
// const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
// const uint32_t PAGE_SIZE = 4096;
#endif
// 通用实现方式
#define ID_SIZE  size_of_attribute(Row, id)                 // ID大小
#define USERNAME_SIZE  size_of_attribute(Row, username)     // 名字大小
#define EMAIL_SIZE size_of_attribute(Row, email)            // 邮箱大小
#define ID_OFFSET 0                                         // ID偏移量，预设为0
#define USERNAME_OFFSET (ID_OFFSET + ID_SIZE)               // 名字偏移量
#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)      // 邮箱偏移量
#define ROW_SIZE (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)     // 行大小
#define PAGE_SIZE 4096                                      // 页大小
// 表的最大页面数
#define TABLE_MAX_PAGES 100                                 
#if 0
// GCC下实现方式
// const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
// const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;
#endif
// 通用实现方式
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)                // 每页行数
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)    // 表的最大行数
// 表
typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];

} Table;










// -----------------------------------
// 函数声明模块
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
#if 0
// 获取指令结果
MetaCommandResult do_meta_command(InputBuffer* input_buffer);
// 获取状态准备结果
PrepareResult prepare_statement(InputBuffer* input_buffer,
	Statement* statement);
// 获取执行结果
void execute_statement(Statement* statement);
// 测试 SQL_VM
void test_SQL_VM();
#endif

// ----------------------------------------
// Hard_Code_Table insert  模块使用函数声明

// 输出行
void print_row(Row* row);
// 序列化
void serialize_row(Row* source, void* destination);
// 反序列化
void deserialize_row(void* source, Row* destination);
// 高效地定位和管理数据页（Page）内部的具体行记录（Row）
void* row_slot(Table* table, uint32_t row_num);
// 创建新表
Table* new_table();
// 释放表内存
void free_table(Table* table);
// 获取指令结果
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table);
// 获取准备状态结果
PrepareResult prepare_statement(InputBuffer* input_buffer,
    Statement* statement);
// 获取执行插入结果
ExecuteResult execute_insert(Statement* statement, Table* table);
// 获取执行选择结果
ExecuteResult execute_select(Statement* statement, Table* table);
// 获取执行状态结果
ExecuteResult execute_statement(Statement* statement, Table* table);
// 测试插入模块功能
void test_Insert();

