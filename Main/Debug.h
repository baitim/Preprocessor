#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#include "../Stack/Stack.h"

long long make_number_canary();

const long long DEFAULT_CANARY = make_number_canary();

#define stack_dump(stk) stack_dump_(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__, #stk) 
Errors stack_dump_(Stack *stack, const char *file, const char *func, int line, const char *stk);

void print_stack_pointers(const Stack *stack);

int get_hash(Stack *stack);

#endif // DEBUG_H
