#include <stdlib.h>
#include <string.h>

#include "Disasm.h"

Errors process_byte_commands(FILE *dest, FILE *src)
{
    if (!src) return ERROR_READ_FILE;
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;
    char *instruction = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
    if (!instruction)
        return ERROR_ALLOC_FAIL;
    while (true) {
        int int_instruct = -1;
        int count_input = fscanf(src, "%d", &int_instruct);
        if (count_input != 1)
            break;

        for (int i = 0; i < COUNT_COMMANDS; i++) {
            if (COMMANDS[i].int_name == int_instruct) {
                fprintf(dest, "%s", COMMANDS[i].name);

                count_input = 0;
                int value = 0;
                if (int_instruct == 1)
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