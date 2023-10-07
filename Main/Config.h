#ifndef CONFIG_H
#define CONFIG_H

typedef int type_el;

const unsigned char POISON_BYTE = 0xbe;
const type_el POISON_EL = POISON_BYTE;

void print_el(const type_el* elem);

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

enum Arg_types {
    NUM = 1 << 1,
    REG = 1 << 2
};

struct Register {
    const char *name;
    int value;
};

const Register REGISTERS[] = {
    {"rax", 0},
    {"rbx", 0},
    {"rcx", 0},
    {"rdx", 0}
};
const int COUNT_REGISTERS = sizeof(REGISTERS) / sizeof(Register);

union SPU {
    struct Register;
    struct Stack;
    struct Data;
};

#endif // CONFIG_H