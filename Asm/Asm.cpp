#include <stdlib.h>
#include <string.h>

#include "Asm.h"
#include "../Config.h"

#define DEF_CMD(name_cmd, num, args, code)                                          \
    if (strcmp(src->pointers[number_string], #name_cmd) == 0)                       \
    {                                                                               \
        fprintf(dest, "%d ", CMD_ ## name_cmd);                                     \
        number_string++;                                                            \
        if (args == 0) {                                                            \
            fprintf(dest, "\n");                                                    \
            continue;                                                               \
        }                                                                           \
        int is_reg = 0;                                                             \
        for (int j = 0; j < COUNT_REGISTERS; j++) {                                 \
            if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {     \
                fprintf(dest, " %d", REGISTERS[j].index);                           \
                is_reg = 1;                                                         \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        if (!is_reg)                                                                \
            fprintf(dest, " %d", atoi(src->pointers[number_string]));               \
        number_string++;                                                            \
        fprintf(dest, "\n");                                                        \
        continue;                                                                   \
    }                                                                               \
    else
Errors process_input_commands_txt(FILE *dest, const Data *src)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    int number_string = 0;
    while (number_string < src->commands_count) {
        #include "../DSL"
        {;}
    }
    return error;
}
#undef DEF_CMD

#define DEF_CMD(name_cmd, num, args, code)                                          \
    if (strcmp(src->pointers[number_string], #name_cmd) == 0)                       \
    {                                                                               \
        *((int *)command + number_string) = num;                                    \
        number_string++;                                                            \
        if (args == 0)                                                              \
            continue;                                                               \
        int is_reg = 0;                                                             \
        for (int j = 0; j < COUNT_REGISTERS; j++) {                                 \
            if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {     \
                *((int *)command + number_string) = REGISTERS[j].index;             \
                is_reg = 1;                                                         \
                break;                                                              \
            }                                                                       \
        }                                                                           \
        if (!is_reg)                                                                \
            *((int *)command + number_string) = atoi(src->pointers[number_string]); \
        number_string++;                                                            \
        continue;                                                                   \
    }                                                                               \
    else
Errors process_input_commands_bin(FILE *dest, const Data *src)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    char *command = (char *)calloc(src->commands_count * sizeof(int), sizeof(char));
    if (!command)
        return ERROR_READ_FILE;

    int number_string = 0;
    while (number_string < src->commands_count) {
        #include "../DSL"
        {;}
    }
    int count_print = (int)fwrite(command, sizeof(command[0]), number_string * sizeof(int), dest);
    if (count_print != src->commands_count * (int)sizeof(int)) {
        free(command);
        return ERROR_WRITE_FILE;
    }
    free(command);
    return error;
}
#undef DEF_CMD