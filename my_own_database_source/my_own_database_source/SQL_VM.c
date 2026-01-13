#define _CRT_SECURE_NO_WARNINGS 1
#include "database.h"
// 2026.01.13记录SQL和VM的C语言实现源代码

#if 0
// 开辟新的输入缓冲区
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}
//InputBuffer* new_input_buffer() {
//    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
//    input_buffer->buffer = NULL;
//}
#endif 

// 获取指令结果
MetaCommandResult do_meta_command(InputBuffer * input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}
// 获取状态准备结果
PrepareResult prepare_statement(InputBuffer * input_buffer,
    Statement * statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}
// 获取执行结果
void execute_statement(Statement * statement) {
    switch (statement->type) {
    case (STATEMENT_INSERT):
        printf("This is where we would do an insert.\n");
        break;
    case (STATEMENT_SELECT):
        printf("This is where we would do a select.\n");
        break;
    }
}

void test_SQL_VM() {
    // 创建输入缓冲区指针
    InputBuffer* input_buffer = new_input_buffer();
    // LOOP
    while (true) {
        // PRINT
        print_prompt();
        // READ
        read_input(input_buffer);
        // EXECUTE
        // 执行退出指令
        if (input_buffer->buffer[0] == '.') {
            // 开关判断具体指令
            switch (do_meta_command(input_buffer)) {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }
        // 执行其他指令
        Statement statement;
        // 获取准备状态
        switch (prepare_statement(input_buffer, &statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",
                input_buffer->buffer);
            continue;
        }
        // 准备成功后，获取执行状态
        execute_statement(&statement);
        // 输出执行结果
        printf("Executed.\n");
    }
}

#if 0
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
//@@ - 40, 17 + 57, 67 @@ void close_input_buffer(InputBuffer * input_buffer) {
//    free(input_buffer);
//}

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
#endif














