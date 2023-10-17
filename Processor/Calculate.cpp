#include <assert.h>
#include <string.h>
#include <math.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "../Commands.h"
#include "../Config.h"
#include "../Output.h"
#include "../Errors.h"
#include "../Asm/Input.h"

#define DEF_CMD(name, num, args, code)  \
    case CMD_ ## name:                  \
        code                            \

void calculate(const char *name_of_file)
{
    FILE *src = fopen(name_of_file, "rb");

    const int size_file = (int)fsize(name_of_file);
    char *commands = (char *)calloc(size_file, sizeof(char));
    if (!commands)
        return;

    int count_read = (int)fread(commands, sizeof(commands[0]), size_file, src);
    if (count_read != size_file)
        return;

    Stack stack = {};
    stack_ctor(&stack);

    int number_command = 0;
    while (number_command * (int)sizeof(int) < size_file) {
        int command = *((int *)commands + number_command);
        number_command++;
        
        switch (command) {
            #include "../DSL"
            default: assert(0);
        }
        //print_commands(name_of_file, number_command);
    }

    stack_dtor(&stack);
    fclose(src);
}
#undef CMD_DEF