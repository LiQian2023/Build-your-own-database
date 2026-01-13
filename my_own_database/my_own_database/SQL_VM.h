#pragma once
#include "REPL.h"

// 指令状态
typedef enum Meta_Command_State {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED
}Meta_Command_state;
// 准备状态
typedef enum Prepare_State {
	PREPARE_SUCCESS,
	PREPARE_RECOGNIZED
}Prepare_State;
// 状态值
typedef enum Statemen{
	STATEMENT_INSERT,
	STATEMENT_SELECT
}Statement;
// 状态类型
typedef struct StatementType {
	Statement type;
}StatementType;


// 获取指令信息
// .exit
// insert
// select
// unrecognized
Meta_Command_state get_meta_command_state(Input_Buffer** input_buffer);

// 获取准备状态
// prepare_succuss
// prepare_unrecognized
Prepare_State get_prepare_state(Input_Buffer** input_buffer, StatementType* statement);

// 获取执行状态
// insert
// select
// unrecognized
void execute_command(Input_Buffer** input_buffer, StatementType* statement);

// SQL_VM测试
void test_SQL_VM();
