#include <assert.h>
#include <string.h>
#include <math.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "../Commands.h"
#include "../Config.h"
#include "../Output.h"

#define DEF_CMD(name, num, args, code)  \
    case CMD_ ## name:                  \
        code                            \

void calculate(const char *name_of_file)
{
    FILE *src = fopen(name_of_file, "r");

    Stack stack = {};
    stack_ctor(&stack);

    int number_command = 1;
    while (true) {
        int command = -1;
        int count_input = fscanf(src, "%d", &command);
        if (count_input != 1)
            break;
        
        switch (command) {
            #include "../Com.txt"
            default: assert(0);
        }
        //print_commands(name_of_file, number_command);

        number_command++;
    }

    stack_dtor(&stack);
    fclose(src);
}
#undef CMD_DEF