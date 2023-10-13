#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Stack/Stack.h"
#include "Errors.h"

long long make_number_canary();

const long long DEFAULT_CANARY = make_number_canary();

#define stack_dump(stk) stack_dump_(stk, __FILE__, __PRETTY_FUNCTION__, __LINE__, #stk) 
Errors stack_dump_(Stack *stack, const char *file, const char *func, int line, const char *stk);

void print_stack_pointers(const Stack *stack);

int get_hash(Stack *stack);

#endif // DEBUG_H
