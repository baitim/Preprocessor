#include <stdlib.h>
#include <string.h>

#include "Asm.h"

Errors process_input_commands(FILE *dest, const Data *src)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    int number_string = 0;
    while (number_string < src->commands_count) {
        for (int i = 0; i < COUNT_COMMANDS; i++) {
            if (strcmp(COMMANDS[i].name, src->pointers[number_string]) == 0) {
                fprintf(dest, "%d", COMMANDS[i].int_name);

                if (COMMANDS[i].arg_types > 0) {
                    number_string++;
                    fprintf(dest, " %s", src->pointers[number_string]);
                }
                
                break;
            }
        }
        number_string++;
        fprintf(dest, "\n");
    }
    return error;
}
