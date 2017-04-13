/* See Copyright Notice in azure.h. */

#ifndef _AERROR_H_
#define _AERROR_H_

#define AZ_NO_ERROR 0	/* everything is ok */

#define AZ_OUT_OF_MEMORY (int)"out of memory"

#define AZ_PARSER_FILE_NOT_FOUND (int)"Compile error: file not found"
#define AZ_PARSER_SYNTAX_ERROR (int)"Compile error: syntax error"
#define AZ_PARSER_NO_LOCAL_FUNCTION (int)"Compile error: no local function"
#define AZ_PARSER_UNKNOWN_ERROR (int)"Compile error: unknown error"
#define AZ_PARSER_UNDEFINED (int)"Compile error: undefined error"
#define AZ_PARSER_DIFFERENT_NUMOFARGS (int)"Compile error: different number of arguments"
#define AZ_PARSER_UNKNOWN_ESCAPE (int)"Compile error: unknown escape character"
#define AZ_PARSER_REDEFINED (int)"Compile error: redefinition"
#define AZ_PARSER_INCLUDE_NOT_FOUND (int)"Compile error: include file not found\n"

#define AZ_VM_HALT (int)"vm halts"
#define AZ_VM_OUT_OF_MEMORY (int)"Run-time error: vm out of memory"
#define AZ_VM_CODE_VIOLATION (int)"Run-time error: vm code violation"
#define AZ_VM_DATA_VIOLATION (int)"Run-time error: vm data violation"
#define AZ_VM_DIV_ZERO (int)"Run-time error: vm divided by zero"
#define AZ_VM_STACK_OVERFLOW (int)"Run-time error: vm stack overflow"
#define AZ_VM_STACK_UNDERFLOW (int)"Run-time error: vm stack underflow"
#define AZ_VM_HEAP_ALLOCATION_ERROR (int)"Run-time error: vm heap allocation error"
#define AZ_VM_HEAP_AVAILABLETABLE_CORRUPT (int)"Run-time error: vm heap available table corrupt"
#define AZ_VM_HEAP_ALLOCATIONTABLE_CORRUPT (int)"Run-time error: vm heap allocation table corrupt"
#define AZ_VM_INVALID_OPERATION (int)"Run-time error: vm invalid operation"
#define AZ_VM_CONTEXT_CORRUPTED (int)"Run-time error: vm context corrupted"
#define AZ_VM_THREAD_STACK_OVERFLOW (int)"Run-time error: vm thread stack overflow"
#define AZ_VM_THREAD_STACK_UNDERFLOW (int)"Run-time error: vm thread stack underflow"
#define AZ_VM_UNHANDLED_EXCEPTION (int)"Run-time error: vm unhandled exception"
#define AZ_VM_CANNOT_LOAD_LIB (int)"Run-time error: cannot load library"
#define AZ_VM_CANNOT_FIND_FUNC (int)"Run-time error: cannot locate function entry.\n"
#define AZ_VM_INCORRECT_TYPE (int)"Run-time error: incorrect data type.\n"
#define AZ_VM_INVALID_ARGUMENT (int)"Run-time error: invalid argument.\n"
#define AZ_VM_INVALID_TABLE (int)"Run-time error: invalid table.\n"

int error(const char* msg);
int warning(const char* msg);

#endif
