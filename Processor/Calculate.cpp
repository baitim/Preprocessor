#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "../Config.h"
#include "../Output.h"
#include "../Errors.h"
#include "../Asm/Input.h"

#define FREE_BYTES 5
#define NUN 0
#define REG (1 << 5)
#define NUM (1 << 6)
#define MEM (1 << 7)
#define STR (1 << 8)
#define PRECISION 100
#define PUSH(arg)       stack_push(&stack, (arg))
#define POP(arg)        stack_pop (&stack, (arg))
#define PUSH_COM(arg)   stack_push(&stack_commands, (arg));
#define POP_COM(arg)    stack_pop (&stack_commands, (arg));

#define IS_REG (command & REG)
#define IS_NUM (command & NUM)
#define IS_MEM (command & MEM)

// #define PRINT_COMMANDS

#define DEF_CMD(name, num, type_args, args, code)       \
    case CMD_ ## name:                                  \
        code

GlobalErrors calculate(const char *name_of_file)
{
    FILE *src = fopen(name_of_file, "rb");
    if (!src) return GLOBAL_ERROR_READ_FILE;

    int size_file = 0;
    if (fsize(&size_file, name_of_file))
        return GLOBAL_ERROR_READ_FILE;
        
    size_file /= sizeof(int);

    int *commands = (int *)calloc(size_file + MAGIC_INTS, sizeof(int));
    if (!commands)
        return GLOBAL_ERROR_NO;

    int count_read = (int)fread(commands, sizeof(commands[0]), size_file, src);
    if (count_read != size_file)
        return GLOBAL_ERROR_NO;

    Stack stack = {};
    stack_ctor(&stack);

    Stack stack_commands = {};
    stack_ctor(&stack_commands);

    int ram[MAX_SIZE_RAM] = {};
    for (int i = START_DRAW_RAM; i <= END_DRAW_RAM; i++)
        ram[i] = POISON_DRAW;

    int number_command = MAGIC_INTS;
    while (number_command < size_file) {
        int command = commands[number_command];
        number_command++;

        // M R N _______
        // if (command & MEMORY_BIT)
        // {
        //     get_memory_arg()
        // }
        // ...
        
        // int* value = get_arg(commands);

        switch ((int)command % (int)powf(2, FREE_BYTES)) {
            #include "../Codegen.inc.h"
            default: {
                printf(print_lred("DEFAULT CASE!\nERROR in %s %s %d\n"),
                __FILE__, __PRETTY_FUNCTION__, __LINE__);
                printf(print_lred("Number command = %d\n"), number_command);
                return GLOBAL_ERROR_NO;
                };

        }
        //print_commands(name_of_file, number_command);
    }

    free(commands);
    stack_dtor(&stack_commands);
    stack_dtor(&stack);
    fclose(src);
    return GLOBAL_ERROR_NO;
}
#undef CMD_DEF

static int powf(int x, int p)
{
    if (p == 0) return 1;
    if (p % 2 == 1) return x * powf(x, p - 1);
    int z = powf(x, p/2);
    return z * z;
}