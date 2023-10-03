#ifndef COMMANDS_H
#define COMMANDS_H

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

#endif // COMMANDS_H
