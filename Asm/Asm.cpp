#include <stdlib.h>
#include <string.h>
#include <cstdio>

#include "Asm.h"
#include "../Config.h"

#define DEF_CMD(name_cmd, num, type_args, args, code)                                       \
    if (strcmp(src->pointers[number_string], #name_cmd) == 0)                               \
    {                                                                                       \
        number_string++;                                                                    \
        if (args == 0) {                                                                    \
            *((int *)command + index_write) = num;                                          \
            index_write++;                                                                  \
            continue;                                                                       \
        }                                                                                   \
        for (int i = 0; i < args; i++) {                                                    \
            int is_reg = 0;                                                                 \
            int is_label = 0;                                                               \
            for (int j = 0; j < COUNT_REGISTERS; j++) {                                     \
                if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {         \
                    *((int *)command + index_write) = (CMD_ ## name_cmd) + (1 << REG);      \
                    index_write++;                                                          \
                    *((int *)command + index_write) = REGISTERS[j].index;                   \
                    is_reg = 1;                                                             \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
                                                                                            \
            const int len_command = (int)strlen(src->pointers[number_string]);              \
            if (src->pointers[number_string][len_command-1] == ':') {                       \
                *((int *)command + index_write) = (CMD_ ## name_cmd) + (1 << LAB);          \
                index_write++;                                                              \
                for (int j = 0; j < MAX_COUNT_LABELS; j++) {                                \
                    if (!LABELS[j].name) break;                                             \
                    if (strcmp(src->pointers[number_string], LABELS[j].name) == 0) {        \
                        *((int *)command + index_write) = LABELS[j].index;                  \
                        is_label = 1;                                                       \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                if (!is_label) {                                                            \
                    *((int *)command + index_write) = POISON_LABEL;                         \
                    fixup[number_fixup] = number_string;                                    \
                    number_fixup++;                                                         \
                    is_label = 1;                                                           \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if (!is_reg && !is_label) {                                                     \
                *((int *)command + index_write) = (CMD_ ## name_cmd) + (1 << NUM);          \
                index_write++;                                                              \
                *((int *)command + index_write) = atoi(src->pointers[number_string]);       \
            }                                                                               \
        }                                                                                   \
        index_write++;                                                                      \
        number_string++;                                                                    \
        continue;                                                                           \
    }                                                                                       \
    else
Errors process_input_commands_bin(FILE *dest, const Data *src, FILE *labels, int *fixup, int *count_fixup)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    char *command = (char *)calloc(src->commands_count * sizeof(int), sizeof(char));
    if (!command)
        return ERROR_READ_FILE;

    int number_lable = 0;
    int number_string = 0;
    int index_write = 0;
    int number_fixup = 0;
    while (number_string < src->commands_count) {
        #include "../DSL"
        {
            const int len_command = (int)strlen(src->pointers[number_string]);
            if (src->pointers[number_string][len_command-1] == ':') {
                LABELS[number_lable] = (Label){src->pointers[number_string], index_write};
                fprintf(labels, "%s\n%d\n", src->pointers[number_string], index_write);
                number_string++;
                number_lable++;
            }
        }
    }
    *count_fixup = number_fixup;
    fixup = (int *)realloc(fixup, number_fixup * sizeof(int));
    if (!fixup)
        return ERROR_READ_FILE;

    command = (char *)realloc(command, index_write * sizeof(int));
    if (!command)
        return ERROR_READ_FILE;

    int count_print = (int)fwrite(command, sizeof(command[0]), index_write * sizeof(int), dest);
    if (count_print != index_write * (int)sizeof(int)) {
        free(command);
        return ERROR_WRITE_FILE;
    }
    free(command);
    return error;
}
#undef DEF_CMD

Errors process_fixup(const Data *src, const char *bin_file, int *fixup, int count_fixup)
{
    FILE *bin_stream = fopen(bin_file, "r+");
    if (!bin_stream)
        return ERROR_READ_FILE;

    for (int i = 0; i < count_fixup; i++) {
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strcmp(src->pointers[fixup[i]], LABELS[j].name) == 0) {
                fseek(bin_stream, (fixup[i] - i - 1) * (int)sizeof(int), SEEK_SET);
                int new_value[1] = {};
                new_value[0] = LABELS[j].index;
                int count_write = (int)fwrite(new_value, sizeof(int), 1, bin_stream);

                if (count_write != 1) {
                    fclose(bin_stream);
                    return ERROR_WRITE_FILE;
                }
                break;
            }
        }
    }

    fclose(bin_stream);
    return ERROR_NO;
}