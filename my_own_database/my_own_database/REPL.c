#define _CRT_SECURE_NO_WARNINGS 1
#include "REPL.h"
// 2026.01.12自己实现的REPL

// 初始化输入缓冲区指针
Input_Buffer* new_input_buffer() {
	Input_Buffer* input_buffer = (Input_Buffer*)calloc(1, sizeof(Input_Buffer));
	if (input_buffer == NULL) {
		perror("calloc");
	}
	else {
		input_buffer->buffer = NULL;
		input_buffer->buffer_length = 0;
		input_buffer->input_length = 0;
	}
	return input_buffer;
}

// PRINT
Print_promp() {
	printf(">:");
}

// 获取当前缓冲区一行长度
ssize_t my_getline(char** buffer, size_t* buffer_length, FILE* stream) {
	// 传参错误：-3
	if (stream == NULL||buffer_length == NULL||buffer == NULL) {
		printf("参数无效\n");
		return -3;
	}
	if (*buffer_length == 0) {
		*buffer_length = 32;
		// 字符串空间申请失败：-2
		*buffer = (char*)calloc(*buffer_length, sizeof(char));
		if (*buffer == NULL) {
			perror("calloc:*buffer");
			return -2;
		}
	}

	ssize_t read_length = 0;
	int ch = 0;

	while ((ch = fgetc(stream)) != EOF) {
		(*buffer)[read_length] = ch;
		// 扩容
		if (read_length + 1 >= *buffer_length) {
			*buffer_length += 16;
			char* tmp = (char*)realloc(*buffer, *buffer_length * sizeof(char));
			if (tmp == NULL) {
				perror("realloc:*buffer");
				return -2;
			}
			else {
				*buffer = tmp;
			}
		}
		read_length += 1;

		// 读取到'\n' 或者 '\0'结束读取
		if (ch == '\n' || ch == '\0') {
			break;
		}
	}
	return read_length;
}

// READ
void Read_input(Input_Buffer** input_buffer) {
	if (input_buffer == NULL || (*input_buffer) == NULL) {
		printf("参数无效\n");
		return;
	}
	// 获取一行的字符
	ssize_t read_byte = my_getline(&((*input_buffer)->buffer), &((*input_buffer)->buffer_length), stdin);

	if (read_byte <= 0) {
		printf("读取失败\n");
		close_input_buffer(input_buffer);
		exit(EXIT_FAILURE);
	}
	else {
		(*input_buffer)->input_length = read_byte - 1;
		(*input_buffer)->buffer[read_byte - 1] = 0;
	}
}
// 关闭输入缓冲区
void close_input_buffer(Input_Buffer** input_buffer) {
	free((*input_buffer)->buffer);
	(*input_buffer)->buffer = NULL;
	free(*input_buffer);
	*input_buffer = NULL;
}
// 打印输入缓冲区
void Print_read(Input_Buffer** input_buffer) {
	printf("当前读取的缓冲区内容：");
	for (int i = 0; i < (*input_buffer)->input_length; i++) {
		printf("%c", (*input_buffer)->buffer[i]);
	}
	printf("\n");
	printf("当前读取的缓冲区大小：%zu\n", (*input_buffer)->buffer_length);
	printf("当前读取的内容总数量：%lld\n", (*input_buffer)->input_length);
}

// REPL测试
void test_REPL() {
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
		if (strcmp(input_buffer->buffer, ".exit") == 0) {
			close_input_buffer(&input_buffer);
			exit(EXIT_SUCCESS);
		}
		else {
			printf("无法识别当前指令：%s\n", input_buffer->buffer);
		}
	}
}