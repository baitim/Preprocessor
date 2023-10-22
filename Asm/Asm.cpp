#include <string.h>
#include <cstdio>
#include <stdlib.h>

#include "Asm.h"
#include "../Config.h"
#include "../Labels.h"

static Label LABELS[MAX_COUNT_LABELS] = {};

enum Type_arg {
    TYPE_ARG_NUM = 1,
    TYPE_ARG_REG = 2,
    TYPE_ARG_LAB = 3
};

static Type_arg check_type_arg(const char *arg);

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
            int type_arg = check_type_arg(src->pointers[number_string]);                    \
            if (type_arg == TYPE_ARG_REG) {                                                          \
                for (int j = 0; j < COUNT_REGISTERS; j++) {                                 \
                    if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {     \
                        *((int *)command + index_write) = (CMD_ ## name_cmd) + (1 << REG);  \
                        index_write++;                                                      \
                        *((int *)command + index_write) = REGISTERS[j].index;               \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
            }                                                                               \
                                                                                            \
            int was_label = 0;                                                              \
            if (type_arg == TYPE_ARG_LAB) {                                                 \
                *((int *)command + index_write) = (CMD_ ## name_cmd) + (1 << LAB);          \
                index_write++;                                                              \
                for (int j = 0; j < MAX_COUNT_LABELS; j++) {                                \
                    if (!LABELS[j].name) break;                                             \
                    if (strcmp(src->pointers[number_string], LABELS[j].name) == 0) {        \
                        *((int *)command + index_write) = LABELS[j].index;                  \
                        was_label = 1;                                                      \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                if (!was_label) {                                                           \
                    *((int *)command + index_write) = POISON_LABEL;                         \
                    pointers_labels[number_fixup].in_src = number_string;                  \
                    pointers_labels[number_fixup].in_bin = index_write;                    \
                    number_fixup++;                                                         \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if (type_arg == TYPE_ARG_NUM) {                                                 \
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
Errors process_input_commands_bin(FILE *dest, const Data *src, FILE *labels, Pointers_label *pointers_labels, int *count_fixup)
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
    int empty_strings = 0;
    printf("src->commands_count = %d\n", src->commands_count);
    while (number_string < src->commands_count) {
        printf("number_string = %d\n", number_string);
        if (strlen(src->pointers[number_string]) == 0) { 
            number_string++;
            empty_strings++;
            continue;
        }
        
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

Errors process_fixup(const Data *src, const char *bin_file, Pointers_label *pointers_labels, int count_fixup)
{
    FILE *bin_stream = fopen(bin_file, "r+");
    if (!bin_stream)
        return ERROR_READ_FILE;
    for (int i = 0; i < count_fixup; i++) {
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strcmp(src->pointers[pointers_labels[i].in_src], LABELS[j].name) == 0) {
                fseek(bin_stream, (pointers_labels[i].in_bin - i) * (int)sizeof(int), SEEK_SET);
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


Type_arg check_type_arg(const char *arg)
{
    const int len_arg = (int)strlen(arg);
    if (arg[len_arg-1] == ':') return TYPE_ARG_LAB;

    for (int j = 0; j < COUNT_REGISTERS; j++) {
        if (strcmp(arg, REGISTERS[j].name) == 0)
            return TYPE_ARG_REG;
    }
    return TYPE_ARG_NUM;
}