#ifndef OUTPUT_H
#define OUTPUT_H

#include "Stack/Stack.h"
#include "Errors.h"

#define dump(err) dump_(err, __FILE__, __PRETTY_FUNCTION__, __LINE__) 
void dump_(int err, const char *file, const char *func, int line);

void print_stack(Stack *stack);

void print_commands(const char *name_of_file, int command);

#endif // OUTPUT_H