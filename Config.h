#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>

typedef int type_el;

const int MAX_SIZE_RAM =    1501;
const int START_DRAW_RAM  = 500;
const int END_DRAW_RAM    = 1500;

const int MAX_SIZE_ARGUMENT = 100;
const int MAX_SIZE_COMMAND  = 1;

const int MAGIC_BYTES = 16;
const char MAGIC_DATA[MAGIC_BYTES + 1] = "TIMUR BAIDUSENOV";
const int MAGIC_INTS = (MAGIC_BYTES + 3) / 4;

const unsigned char POISON_BYTE = 0xbe;
const type_el POISON_EL = POISON_BYTE;
const int POISON_DRAW = -0xaab;

void print_el(const type_el* elem);

#define DEF_CMD(name, num, type_args, args, code) \
    CMD_ ## name = num,
enum Commands {
    #include "Codegen.inc.h"
};
#undef DEF_CMD

struct Register {
    const char *name;
    int value;
    int index;
};
static Register REGISTERS[] = {
    {"rax", POISON_BYTE, 0xAA},
    {"rbx", POISON_BYTE, 0xAB},
    {"rcx", POISON_BYTE, 0xAC},
    {"rdx", POISON_BYTE, 0xAD},
    {"rex", POISON_BYTE, 0xAE},
};
const int COUNT_REGISTERS = sizeof(REGISTERS) / sizeof(Register);

#endif // CONFIG_H