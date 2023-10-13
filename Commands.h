#ifndef COMMANDS_H
#define COMMANDS_H

struct Input_commands {
    const char *name;
    int opcode;
    int arg_types;
    const char *description;
};

const int MAX_SIZE_COMMAND = 10;

enum Commands {
    PUSH = 43,
    IN =   2,
    POP =  33,
    ADD =  4,
    SUB =  5,
    MUL =  6,
    DIV =  7,
    SQRT = 8,
    SIN =  9,
    COS =  10,
    OUT =  11,
    HLT =  12,
};

enum Arg_types {
    NUN = 0,
    NUM = 1 << 1,
    REG = 1 << 2,
};

const Input_commands COMMANDS[] = {
    {"push",    PUSH, NUM | REG,     "add element to stack"},
    {"in",      IN,   NUN,           "choose and add element to stack"},
    {"pop",     POP,  REG,           "write register"},
    {"add",     ADD,  NUN,           "sum elements"},
    {"sub",     SUB,  NUN,           "subtraction elements"},
    {"mul",     MUL,  NUN,           "multiplicate elements"},
    {"div",     DIV,  NUN,           "divide elements"},
    {"sqrt",    SQRT, NUN,           "sqrt(element)"},
    {"sin",     SIN,  NUN,           "sin(element)"},
    {"cos",     COS,  NUN,           "cos(element)"},
    {"out",     OUT,  NUN,           "get answer"},
    {"HLT",     HLT,  NUN,           "change equation"}
};
const int COUNT_COMMANDS = sizeof(COMMANDS) / sizeof(Input_commands);

#endif // COMMANDS_H
