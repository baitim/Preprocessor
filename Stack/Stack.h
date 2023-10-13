#ifndef STACK_H
#define STACK_H

#include "../Config.h"
#include "../Errors.h"

struct Stack {
    long long left_canary_struct;
    type_el *data;
    int size;
    int capacity;
    int hash;
    long long right_canary_struct;
};

#include "../Debug.h"

Errors stack_ctor(Stack *stack);

Errors stack_dtor(Stack *stack);

Errors stack_push(Stack *stack, type_el value);

Errors stack_pop(Stack *stack, type_el *value);

int get_left_canary_index(const Stack */*stack*/);

int get_right_canary_index(const Stack *stack);

#endif // STACK_H
