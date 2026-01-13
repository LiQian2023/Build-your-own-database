#pragma once
#pragma pack(1)		// 修改默认对齐参数为1
#include "REPL.h"
#include <stdint.h>


// 页面管理模块
//-行：
//- ID：大小、偏移量
//- NAME：大小、偏移量
//- EMAIL：大小、偏移量
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct Row {
	uint32_t ID;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
}Row;
// 宏：获取结构体成员大小
#define sizeof_struct_member(Struct, member) sizeof(((Struct*)0)->member)
// 计算偏移量
#define ID_OFFSET 0
#define ID_SIZE sizeof_struct_member(Row, ID)
#define USERNAME_OFFSET (ID_SIZE + ID_OFFSET)
#define USERNAME_SIZE sizeof_struct_member(Row, username)
#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)
#define EMAIL_SIZE sizeof_struct_member(Row, email)

//- 页：
//- 行大小
//- 页面大小
//- 每一页的最大行数
//- 表最大行数
//- 总页面数
#define ROW_SIZE (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)
#define PAGE_SIZE 4096
#define ROWS_PER_PAGE (PAGE_SIZE/ROW_SIZE)
#define TABLE_PAGE 100
#define TABLE_ROW_SIZE (ROWS_PER_PAGE * TABLE_PAGE)


// 插入模块
//- 系统指令结果：
//- 有效系统指令：`.exit`
//- 无法识别指令
typedef enum Meta_Command {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

//- 准备结果
//- 指令准备成功
//- 错误指令
//- 指令无法识别
typedef enum Prepare {
	PREPARE_SUCCESS,
	PREPARE_SYNTAX_ERROR,
	PREPARE_UNRECOGNIZED
}PrepareResult;

//- 状态类型
//- 插入
//- 选择
typedef enum Statement_Type {
	INSERT,
	SELECT
}StatementType;

//- 状态
//- 当前状态类型
//- 行插入
typedef struct Statement {
	StatementType type;
	Row row_insert;
}Statement;

//-执行结果
//- 执行成功
//- 表已满
//- 执行失败
typedef enum Execute {
	EXECUTE_SUCCESS,
	TABLE_FULL,
	EXECUTE_FAIL
}ExecuteResult;

//表管理
//- 行数
//- 页面

typedef struct Table {
	uint32_t row_num;
	void* page[PAGE_SIZE];
}Table;

#pragma pack()	// 恢复内存对齐

// 功能模块
// 输出行信息
void Print_row(Row* row);
// 序列化：将源地址信息转存到目标地址中（`row* -> void* `）
void serialize(Row* source, void* destination);
// 反序列化：将源地址信息转存到目标地址中（`void*->row * `）
void deserialize(void* source, Row* destination);
// 页面管理：`row_slot` 获取行的位置信息
void* row_slot(Table** table, uint32_t row_num);
// 创建新表
Table* new_table();
// 释放表空间：结束使用后，释放表的内存空间
void Free_Table(Table** table);
// 获取系统指令结果
MetaCommandResult do_mete_command(Input_Buffer** input_buffer, Table** table);
// 获取指令准备结果
PrepareResult prepare_command(Input_Buffer** input_buffer, Statement* statement);
// 获取插入指令运行结果
ExecuteResult execute_insert(Table** table, Statement* statement);

// 获取选择指令运行结果
ExecuteResult execute_select(Table** table);
// 获取执行状态结果
ExecuteResult execute(Table** table, Statement* statement);

// 测试插入模块
void test_insert();