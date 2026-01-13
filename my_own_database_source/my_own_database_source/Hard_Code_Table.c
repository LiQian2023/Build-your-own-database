#define _CRT_SECURE_NO_WARNINGS 1

#include "database.h"
// 2026.01.13记录 Hard_Code_Table insert 模块的C语言实现源代码

// 输出行
void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}
// 序列化——将source中的数据转存到void* 类型的 destination中
void serialize_row(Row* source, void* destination) {
    // GCC下实现方式
    // memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    // memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    // memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
    // 通用实现方式
    memcpy((char*)destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy((char*)destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy((char*)destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);

}
// 反序列化——将source中的数据转存到Row* 类型的destination中
void deserialize_row(void* source, Row* destination) {
    // GCC下实现方式
    // memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    // memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    // memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
    // 通用实现方式
    memcpy(&(destination->id), (char*)source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), (char*)source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), (char*)source + EMAIL_OFFSET, EMAIL_SIZE);
}
// 高效地定位和管理数据页（Page）内部的具体行记录（Row）
void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    // 获取row_num的对应页面
    void* page = table->pages[page_num]; 
    // 若当前页面为 NULL，则为该页面开辟空间
    if (page == NULL) {
        // Allocate memory only when we try to access page
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    // 获取行偏移量
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    // 获取字节偏移量
    uint32_t byte_offset = row_offset * ROW_SIZE;
    // return page + byte_offset; —— GCC下实现方式
    // 返回页面偏移量
    return (char*)page + byte_offset; // 通用实现方式
}
// 创建新表
Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    // 初始化新表行数
    table->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        // 初始化新表页面
        table->pages[i] = NULL;

    }
    // 返回新表
    return table;
}
// 释放表内存
void free_table(Table* table) {
    for (int i = 0; table->pages[i]; i++) {
        free(table->pages[i]);
    }
    free(table);
}
//InputBuffer* new_input_buffer() {
//    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
//    input_buffer->buffer = NULL;
//    @@ - 40, 17 + 140, 105 @@ void close_input_buffer(InputBuffer * input_buffer) {
//        free(input_buffer);
//    }

// 获取指令结果
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_table(table);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}
// 获取准备状态结果
PrepareResult prepare_statement(InputBuffer* input_buffer,
    Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
            statement->row_to_insert.username, statement->row_to_insert.email
        );
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;

        }
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}
// 获取执行插入结果
ExecuteResult execute_insert(Statement* statement, Table* table) {
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }
    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;
    return EXECUTE_SUCCESS;
}
// 获取执行选择结果
ExecuteResult execute_select(Statement* statement, Table* table) {
    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);

    }
    return EXECUTE_SUCCESS;
}
// 获取执行状态结果
ExecuteResult execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
    case (STATEMENT_INSERT):
        return execute_insert(statement, table);
    case (STATEMENT_SELECT):
        return execute_select(statement, table);
    }
}
// 测试插入模块功能
void test_Insert() {
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);


        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_SYNTAX_ERROR):
            printf("Syntax error. Could not parse statement.\n");
            continue;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",
                input_buffer->buffer);
            continue;

        }
        switch (execute_statement(&statement, table)) {
        case (EXECUTE_SUCCESS):
            printf("Executed.\n");
            break;
        case (EXECUTE_TABLE_FULL):
            printf("Error: Table full.\n");
            break;
        }
    }
}