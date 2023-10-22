#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>

typedef int type_el;
const int COUNT_BYTES_IN_BINARY_TO_DECRIPTION = 17;
const char FIRST_DATA_IN_BINARY[COUNT_BYTES_IN_BINARY_TO_DECRIPTION] = "TIMUR BAIDUSENOV";
const int COUNT_INTS_IN_BINARY_TO_DECRIPTION = (COUNT_BYTES_IN_BINARY_TO_DECRIPTION + 3) / 4;

const unsigned char POISON_BYTE = 0xbe;
const type_el POISON_EL = POISON_BYTE;

void print_el(const type_el* elem);

const int MAX_SIZE_COMMAND = 1;
#define DEF_CMD(name, num, type_args, args, code) \
    CMD_ ## name = num,
enum Commands {
    #include "DSL"
};
#undef DEF_CMD

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