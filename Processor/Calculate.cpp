#include <assert.h>
#include <string.h>
#include <math.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "../Config.h"
#include "../Output.h"
#include "../Errors.h"
#include "../Asm/Input.h"

#include <math.h>

#define DEF_CMD(name, num, type_args, args, code)       \
    case CMD_ ## name:                                  \
        code                                            \

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
    int xx = 0;
    while (number_command * (int)sizeof(int) < size_file) {
        int command = *((int *)commands + number_command);
        number_command++;

        switch ((int)command % (int)powf(2, FREE_BYTES)) {
            #include "../DSL"
            default: assert(0);
        }
        xx++;
        if (xx == 100) break;
        //print_commands(name_of_file, number_command);
    }

    free(commands);
    stack_dtor(&stack);
    fclose(src);
}
#undef CMD_DEF

static int powf(int x, int p)
{
    if (p == 0) return 1;
    if (p % 2 == 1) return x * powf(x, p - 1);
    int z = powf(x, p/2);
    return z * z;
}