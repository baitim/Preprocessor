#include <stdlib.h>
#include <string.h>

#include "To_byte_code.h"

Errors process_input_commands(FILE *dest, FILE *src)
{
    if (!src) return ERROR_READ_FILE;
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;
    char *instruction = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
    if (!instruction)
        return ERROR_ALLOC_FAIL;
    while (true) {
        int count_input = fscanf(src, "%s", instruction);
        if (count_input != 1)
            break;
        for (int i = 0; i < COUNT_COMMANDS; i++) {
            if (strcmp(commands[i].name, instruction) == 0) {
                fprintf(dest, "%d", commands[i].int_name);
                count_input = 0;
                int value = 0;
                if (strcmp("push", instruction) == 0)
                    count_input = fscanf(src, "%d", &value);

                if (count_input)
                    fprintf(dest, " %d", value);

                break;
            }
        }
        fprintf(dest, "\n");
    }

    free(instruction);
    return error;
}
