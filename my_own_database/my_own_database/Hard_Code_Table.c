#define _CRT_SECURE_NO_WARNINGS 1
#include "Hard_Code_Table.h"
// 2026.01.13自己实现的Hard_Code_Table insert 模块

// 输出行信息
void Print_row(Row* row) {
	printf("%d\t%s\t%s\n", row->ID, row->username, row->email);
}
// 序列化：将源地址信息转存到目标地址中（`row* -> void* `）
void serialize(Row* source, void* destination) {
	memcpy((char*)destination + ID_OFFSET, &(source->ID), ID_SIZE);
	memcpy((char*)destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy((char*)destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}
// 反序列化：将源地址信息转存到目标地址中（`void*->row * `）
void deserialize(void* source,Row* destination) {
	memcpy(&(destination->ID), (char*)source + ID_OFFSET, ID_SIZE);
	memcpy(destination->username, (char*)source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(destination->email, (char*)source + EMAIL_OFFSET, EMAIL_SIZE);
}
// 页面管理：`row_slot` 获取行的位置信息
void* row_slot(Table** table,uint32_t row_num) {
	// 获取页面行信息
	uint32_t row = row_num / ROWS_PER_PAGE;
	// 获取页面信息
	void* page = (*table)->page[row];
	if (page == NULL) {
		page = (void*)calloc(PAGE_SIZE, sizeof(char));
		(*table)->page[row] = page;
	}
	// 获取页面偏移量
	uint32_t page_offset = row_num % ROWS_PER_PAGE;
	// 获取偏移字节数
	uint32_t byte_offset = page_offset * ROW_SIZE;
	return (char*)page + byte_offset;
}
// 创建新表
Table* new_table() {
	Table* table = (Table*)calloc(1, sizeof(Table));
	if (table == NULL) {
		perror("calloc:table");
		exit(EXIT_FAILURE);
	}
	table->row_num = 0;
	for (int i = 0; i < TABLE_PAGE; i++) {
		table->page[i] = NULL;
	}
	return table;
}
// 释放表空间：结束使用后，释放表的内存空间
void Free_Table(Table** table) {
	for (int i = 0; i < PAGE_SIZE; i++) {
		free((*table)->page[i]);
		(*table)->page[i] = NULL;
	}
	free(*table);
	*table = NULL;
}
// 获取系统指令结果
MetaCommandResult do_mete_command(Input_Buffer** input_buffer, Table** table) {
	if (strcmp((*input_buffer)->buffer, ".exit") == 0) {
		close_input_buffer(input_buffer);
		Free_Table(table);
		exit(EXIT_SUCCESS);
	}
	else {
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}
// 获取指令准备结果
PrepareResult prepare_command(Input_Buffer** input_buffer, Statement* statement) {
	if (strncmp((*input_buffer)->buffer, "insert", 6) == 0) {
		statement->type = INSERT;
		int read = sscanf((*input_buffer)->buffer, "insert %d %s %s", &(statement->row_insert.ID), statement->row_insert.username, statement->row_insert.email);
		if (read < 3) {
			return PREPARE_SYNTAX_ERROR;
		}
		return PREPARE_SUCCESS;
	}
	else if (strncmp((*input_buffer)->buffer, "select", 6) == 0) {
		statement->type = SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZED;
}
// 获取插入指令运行结果
ExecuteResult execute_insert(Table** table, Statement* statement) {
	//表满
	if ((*table)->row_num >= TABLE_ROW_SIZE) {
		return TABLE_FULL;
	}
	//表未满
	serialize(&(statement->row_insert), row_slot(table, (*table)->row_num));
	(*table)->row_num += 1;
	return EXECUTE_SUCCESS;
}

// 获取选择指令运行结果
ExecuteResult execute_select(Table** table) {
	Row row = { 0 };
	for (uint32_t i = 0; i < (*table)->row_num; i++) {
		deserialize(row_slot(table, i), &row);
		Print_row(&row);
	}
	return EXECUTE_SUCCESS;
}
// 获取执行状态结果
ExecuteResult execute(Table** table, Statement* statement) {
	switch (statement->type) {
	case(INSERT):
		return execute_insert(table, statement);
	case(SELECT):
		return execute_select(table);
	default:
		printf("未知错误\n");
		break;
	}
	return EXECUTE_FAIL;
}

// 测试插入模块
void test_insert() {
	Table* table = new_table();
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
			switch (do_mete_command(&input_buffer, &table)) {
			case META_COMMAND_SUCCESS:
				continue;
			case META_COMMAND_UNRECOGNIZED_COMMAND:
				printf("无法识别的系统指令：%s\n", input_buffer->buffer);
				continue;
			default:
				printf("未知错误\n");
				continue;
			}
		}

		Statement statement = { 0 };
		switch (prepare_command(&input_buffer, &statement)) {
		case PREPARE_SUCCESS:
			break;
		case(PREPARE_SYNTAX_ERROR):
			printf("指令 %s 错误\n", input_buffer->buffer);
			continue;
		case PREPARE_UNRECOGNIZED:
			printf("无法识别指令：%s\n", input_buffer->buffer);
			continue;
		default:
			printf("出现了未知错误\n");
			continue;
		}
		switch (execute(&table, &statement)) {
		case (EXECUTE_SUCCESS):
			printf("Execute.\n");
			continue;
		case(TABLE_FULL):
			printf("当前表已满，无法插入新内容\n");
			continue;
		case(EXECUTE_FAIL):
			printf("执行失败\n");
			continue;
		}
	}
}