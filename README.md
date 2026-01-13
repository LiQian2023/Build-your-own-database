# Build-your-own-database
Use the C Language to build up a simple database

Source Address: https://github.com/codecrafters-io/build-your-own-x

# Introduce
This is my first open source progect. 

I will use C to build up a simple database.As a complete beginner in databasses, I'll implement each function step by step while learning.

Let's begin.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
2026.01.12 —— Implemented REPL functionality

Read the source code of com/codecrafters-io/build-your-own-database.

Record the source code in the file —— my_own_database_source.


Finish the code by myself.Seven functions I have writen:

- void test_REPL();															// test REPL's all functions
- void Print_promp();														// print promp which can tell user where to scan
- Input_Buffer* new_input_buffer();											// Initial the pointer of input_buffer whose data type is Input_Buffer*
- ssize my_getline(char** buffer, size_t* buffer_length, FILE* stream);		// get all characters in one line of stream
- void close_input_buffer(Input_Buffer** input_buffer);						// free the input buffer pointer's space which is been malloc/calloc/realloc
- void read_input(Input_Buffer** input_buffer);								// record string in the stream of stdin
- void Print_read(Input_Buffer** input_buffer);								// print the string、buffer_legnth and input_length the read_input function have read in stdin

Because my operating system is Windows, I have to implement the function my_getline. If your OS is Linux or MacOS, you can use the function getline.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2026.01.13 —— Implemented SQL_VM functionality

Read the source code of com/codecrafters-io/build-your-own-database.

Record the source code in the file —— my_own_database_source.


Finish the code by myself.Four functions I have writen:

- Meta_Command_state get_meta_command_state(Input_Buffer** input_buffer);					// get command statement
- Prepare_State get_prepare_state(Input_Buffer** input_buffer, StatementType* statement);	// get prepare statement
- void execute_command(Input_Buffer** input_buffer, StatementType* statement);				// execute command
- void test_SQL_VM();																		// test SQL_VM's all functions

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------