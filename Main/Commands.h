#ifndef COMMANDS_H
#define COMMANDS_H

struct Input_commands {
    const char *name;
    int int_name;
    int arg_types;
    const char *description;
};

const int MAX_SIZE_COMMAND = 10;

enum Arg_types {
    NUM = 1 << 1,
    REG = 1 << 2
};

const Input_commands COMMANDS[] = {
    {"push",    1,  NUM | REG,    "add element to calculate"},
    {"in",      2,  0,            "choose and add element to calculate"},
    {"pop",     3,  REG,          "write register"},
    {"add",     4,  0,            "sum elements"},
    {"sub",     5,  0,            "subtraction elements"},
    {"mul",     6,  0,            "multiplicate elements"},
    {"div",     7,  0,            "divide elements"},
    {"sqrt",    8,  0,            "sqrt(element)"},
    {"sin",     9,  0,            "sin(element)"},
    {"cos",     10, 0,            "cos(element)"},
    {"out",     11, 0,            "get answer"},
    {"HLT",     12, 0,            "change equation"}
};
const int COUNT_COMMANDS = sizeof(COMMANDS) / sizeof(Input_commands);

#endif // COMMANDS_H
