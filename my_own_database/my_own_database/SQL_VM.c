#define _CRT_SECURE_NO_WARNINGS 1

#include "SQL_VM.h"

// 2026.01.13自己实现的SQL_VM

// 获取指令信息
// .exit
// insert
// select
// unrecognized
Meta_Command_state get_meta_command_state(Input_Buffer** input_buffer) {
	if (strcmp((*input_buffer)->buffer, ".exit") == 0) {
		close_input_buffer(input_buffer);
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED;
	}
}

// 获取准备状态
// prepare_succuss
// prepare_unrecognized
Prepare_State get_prepare_state(Input_Buffer** input_buffer, StatementType* statement) {
	if (strncmp((*input_buffer)->buffer, "insert", 6) == 0) {
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}
	else if (strncmp((*input_buffer)->buffer, "select", 6) == 0) {
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_RECOGNIZED;
}

// 获取执行状态
// insert
// select
// unrecognized
void execute_command(Input_Buffer** input_buffer, StatementType* statement) {
	switch (statement->type) {
	case 	STATEMENT_INSERT:
		printf("execute %s command\n", (*input_buffer)->buffer);
		break;
	case STATEMENT_SELECT:
		printf("execute %s command\n", (*input_buffer)->buffer);
		break;
	default:
		printf("未知错误\n");
		break;
	}
}


// SQL_VM测试
void test_SQL_VM() {
	Input_Buffer* input_buffer = new_input_buffer();
	// LOOP
	while (true) {
		// PRINT
		Print_promp();
		// READ
		Read_input(&input_buffer);
		// PRINT
		Print_read(&input_buffer);

		// EXECUTE
		if (input_buffer->buffer[0] == '.') {
			switch (get_meta_command_state(&input_buffer)) {
			case META_COMMAND_SUCCESS:
				break;
			case META_COMMAND_UNRECOGNIZED:
				printf("无法识别指令：%s\n", input_buffer->buffer);
				continue;
			default:
				printf("未知错误\n");
				continue;
			}
		}

		StatementType statement;
		switch (get_prepare_state(&input_buffer, &statement)) {
		case PREPARE_SUCCESS:
			execute_command(&input_buffer, &statement);
			break;
		case PREPARE_RECOGNIZED:
			printf("无法识别指令：%s\n", input_buffer->buffer);
			continue;
		default:
			printf("出现了未知错误\n");
			continue;
		}
		printf("Execute.\n");
	}
}