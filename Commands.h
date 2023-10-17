#ifndef COMMANDS_H
#define COMMANDS_H

struct Input_commands {
    const char *name;
    int opcode;
    int arg_types;
};

#define DEF_CMD(name, num, args, code) \
    CMD_ ## name = num,
enum Commands {
    #include "DSL"
};
#undef DEF_CMD

enum Arg_types {
    NUN = 0,
    NUM = 1 << 1,
    REG = 1 << 2,
};

const int MAX_SIZE_COMMAND = 1;

#endif // COMMANDS_H
