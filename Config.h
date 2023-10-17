#ifndef CONFIG_H
#define CONFIG_H

typedef int type_el;

const unsigned char POISON_BYTE = 0xbe;
const type_el POISON_EL = POISON_BYTE;

void print_el(const type_el* elem);

const int MAX_SIZE_COMMAND = 1;
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

struct Register {
    const char *name;
    int value;
    int index;
};

static Register REGISTERS[] = {
    {"rax", 0, 0xAA},
    {"rbx", 0, 0xAB},
    {"rcx", 0, 0xAC},
    {"rdx", 0, 0xAD}
};
const int COUNT_REGISTERS = sizeof(REGISTERS) / sizeof(Register);

#endif // CONFIG_H