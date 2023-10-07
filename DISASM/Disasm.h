#ifndef FROM_BYTE_CODE_H
#define FROM_BYTE_CODE_H

#include <stdio.h>

enum Errors {
    ERROR_NO =                          0,
    ERROR_STACK_EMPTY =                 1 << 0,
    ERROR_ALLOC_FAIL =                  1 << 1,
    ERROR_STACK_CAPACITY =              1 << 2,
    ERROR_STACK_SIZE =                  1 << 3,
    ERROR_STACK_CAPACITY_LESS_SIZE =    1 << 4,
    ERROR_LEFT_CANARY_STRUCT =          1 << 5,
    ERROR_RIGHT_CANARY_STRUCT =         1 << 6,
    ERROR_HASH =                        1 << 7,
    ERROR_LEFT_CANARY_DATA =            1 << 8,
    ERROR_RIGHT_CANARY_DATA =           1 << 9,
    ERROR_STACK_DATA_EMPTY =            1 << 10,
    ERROR_READ_FILE =                   1 << 11
};

struct Input_commands {
    const char *name;
    int int_name;
    const char *description;
};

const int MAX_SIZE_COMMAND = 10;

const Input_commands commands[] = {
    {"push",    1,  "add element to calculate"},
    {"add",     2,  "sum elements"},
    {"sub",     3,  "subtraction elements"},
    {"mul",     4,  "multiplicate elements"},
    {"div",     5,  "divide elements"},
    {"sqrt",    6,  "sqrt(element)"},
    {"sin",     7,  "sin(element)"},
    {"cos",     8,  "cos(element)"},
    {"in",      9,  "choose and add element to calculate"},
    {"out",     10, "get answer"},
    {"HLT",     11, "change equation"}
};
const int COUNT_COMMANDS = sizeof(commands) / sizeof(Input_commands);

Errors process_byte_commands(FILE *dest, FILE *src);

#endif // FROM_BYTE_CODE_H