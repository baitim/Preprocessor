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
    TYPE_ARG_LAB = 3,
    TYPE_ARG_MEM = 4
};

static GlobalErrors get_arg(const DATA *src, int *number_char, int *command, 
                      int *index_write, int *number_fixup, int number_string, 
                      Pointers_label *pointers_labels, FILE *listing);

static Type_arg check_type_arg(const char *arg);

static int check_empty(const char *string)
{
    int i = 0;
    while (string[i] == ' ')
        i++;

    if ((int)strlen(string) == i)
        return 1;
    return 0;
}

static GlobalErrors read_len_arg(int *count_spaces, int *len_arg, char *str);

static GlobalErrors get_spaces_lr(int *l_spaces, int *r_spaces, const char *str);

#define NUN 0
#define REG (1 << 5)
#define NUM (1 << 6)
#define MEM (1 << 7)

#define DEF_CMD(name_cmd, num, type_args, args, code)                                       \
    if (strncmp(&src->pointers[number_string][number_char], #name_cmd, len_command) == 0)   \
    {                                                                                       \
        fprintf(listing, "%s\t", #name_cmd);                                                \
        command[index_write] = num;                                                         \
        number_char += len_command;                                                         \
        if (args > 0) {                                                                     \
            error = get_arg(src, &number_char, command, &index_write,                       \
                            &number_fixup, number_string, pointers_labels,                  \
                            listing);                                                       \
            if (error) return error;                                                        \
            fprintf(listing, "%d\t%d\t", num, command[index_write]);                        \
        } else {                                                                            \
            fprintf(listing, "-\t%d\t-\t", num);                                            \
        }                                                                                   \
        index_write++;                                                                      \
    }                                                                                       \
    else

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, 
                                        Pointers_label *pointers_labels, int *count_fixup,
                                        FILE *listing)
{
    GlobalErrors error = GLOBAL_ERROR_NO;
    if (!dest) return GLOBAL_ERROR_READ_FILE;

    fprintf(listing, "index\tnum_str\tcommand\targ\tbin_com\tbin_arg\n");

    int *command = (int *)calloc(2 * (src->commands_count + MAGIC_INTS), sizeof(int));
    if (!command)
        return GLOBAL_ERROR_READ_FILE;

    command = (int *)memcpy(command, MAGIC_DATA, MAGIC_BYTES);
    if (!command)
        return GLOBAL_ERROR_READ_FILE;

    int number_lable = 0;
    int number_string = 0;
    int index_write = MAGIC_INTS;
    int number_fixup = 0;
    while (number_string < src->commands_count) {

        char *comment = strchr(src->pointers[number_string], '/');
        if (comment)
            comment[0] = '\0';

        if (strlen(src->pointers[number_string]) == 0) { number_string++; continue; }
        if (check_empty(src->pointers[number_string])) { number_string++; continue; }

        if (strlen(src->pointers[number_string]) == 0)

        {
            number_string++;
            continue;
        }
        if (check_empty(src->pointers[number_string]))
        {
            number_string++;
            continue;
        }


        fprintf(listing, "%d\t", index_write - MAGIC_INTS);
        fprintf(listing, "%d\t", number_string);
        int number_char = 0;
        while (src->pointers[number_string][number_char] == ' ')
            number_char++;

        int len_command = 0;
        while(src->pointers[number_string][number_char + len_command] != '\n' &&
              src->pointers[number_string][number_char + len_command] != '\0' &&
              src->pointers[number_string][number_char + len_command] != ' ')
            len_command++;

        #include "../Codegen.inc.h"
        {
            int len_lable = 0, count_arg_spaces = 0;
            error = read_len_arg(&count_arg_spaces, &len_lable, &src->pointers[number_string][number_char]);
            if (error) return error;

            if (src->pointers[number_string][number_char + len_lable - 1] == ':') {
                LABELS[number_lable] = (Label){&src->pointers[number_string][number_char], index_write};
                fprintf(listing, "%s\t", &src->pointers[number_string][number_char]);
                fprintf(labels, "%s\n%d\n", &src->pointers[number_string][number_char], index_write);
                number_lable++;
            }
        }
        fprintf(listing, "\n");
        number_string++;
    }
    *count_fixup = number_fixup;

    command = (int *)realloc(command, index_write);
    if (!command)
        return GLOBAL_ERROR_READ_FILE;

    int count_print = (int)fwrite(command, sizeof(command[0]), index_write, dest);
    if (count_print != index_write) {
        free(command);
        return GLOBAL_ERROR_WRITE_FILE;
    }
    free(command);
    return GLOBAL_ERROR_NO;
}
#undef DEF_CMD

GlobalErrors process_fixup(const DATA *src, const char *bin_file, 
                           Pointers_label *pointers_labels, int count_fixup)
{
    // Zachem ty ego otkryl?
    FILE *bin_stream = fopen(bin_file, "r+");
    if (!bin_stream)
        return GLOBAL_ERROR_READ_FILE;

    printf("FIXUP:\n");

    printf("Need count fixups: %d\n", count_fixup);

    for (int i = 0; i < count_fixup; i++) {
        printf("Need fix: %s\n", &src->pointers[pointers_labels[i].in_src][pointers_labels[i].start]);
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strncmp(&src->pointers[pointers_labels[i].in_src][pointers_labels[i].start], LABELS[j].name, pointers_labels[i].len) == 0) {
                fseek(bin_stream, (pointers_labels[i].in_bin) * (int)sizeof(int), SEEK_SET);
                //???
                int new_value[1] = {};
                new_value[0] = LABELS[j].index;
                printf("Fixed: %s %d\n", LABELS[j].name, LABELS[j].index);
                int count_write = (int)fwrite(new_value, sizeof(int), 1, bin_stream);
                if (count_write != 1) {
                    fclose(bin_stream);
                    return GLOBAL_ERROR_WRITE_FILE;
                }
                break;
            }
        }
    }

    fclose(bin_stream);
    return GLOBAL_ERROR_NO;
}

static GlobalErrors get_arg(const DATA *src, int *number_char, int *command, 
                            int *index_write, int *number_fixup, int number_string, 
                            Pointers_label *pointers_labels, FILE *listing)
{
    GlobalErrors error = GLOBAL_ERROR_NO;

    int int_arg = 0;
    int count_read = sscanf(&src->pointers[number_string][*number_char], "%d", &int_arg);
    if (count_read == 1) {
        command[*index_write] += NUM;
        (*index_write)++;
        command[*index_write] = int_arg;
        fprintf(listing, "%d\t", int_arg);
        return GLOBAL_ERROR_NO;
    }

    char *argument = (char *)calloc(MAX_SIZE_ARGUMENT, sizeof(char));
    if (!argument)
        return GLOBAL_ERROR_ALLOC_FAIL;
    int len_arg = 0;
    int count_arg_spaces = 0;
    error = read_len_arg(&count_arg_spaces, &len_arg, &src->pointers[number_string][*number_char]);
    if(error) return error;

    argument = (char *)realloc(argument, len_arg * sizeof(char));
    if (!argument)
        return GLOBAL_ERROR_ALLOC_FAIL;

    memcpy(argument, &src->pointers[number_string][*number_char + count_arg_spaces], len_arg);
    fprintf(listing, "%s\t", argument);
    int type_arg = check_type_arg(argument);
    if (type_arg == TYPE_ARG_MEM) {
        command[*index_write] += MEM;
        int l_space = 1, r_space = 1;
        error = get_spaces_lr(&l_space, &r_space, argument);
        if (error) return error;

        const int len_mem_command = len_arg - l_space - r_space;
        char *memory_command = (char *)calloc(len_mem_command, sizeof(char));
        if (!memory_command)
        return GLOBAL_ERROR_ALLOC_FAIL;
        memcpy(memory_command, argument + l_space, len_mem_command);
        int type_mem_arg = check_type_arg(memory_command);
        if (type_mem_arg == TYPE_ARG_REG) {
            for (int j = 0; j < COUNT_REGISTERS; j++) {
                if (strcmp(memory_command, REGISTERS[j].name) == 0) {
                    command[*index_write] += REG;
                    (*index_write)++;
                    command[*index_write] = REGISTERS[j].index;
                    break;                                                              
                }
            }
        }
        if (type_mem_arg == TYPE_ARG_NUM) {
            command[*index_write] += NUM;
            (*index_write)++;
            command[*index_write] = atoi(memory_command);
        }
    }
 
    if (type_arg == TYPE_ARG_REG) {
        for (int j = 0; j < COUNT_REGISTERS; j++) {
            if (strcmp(argument, REGISTERS[j].name) == 0) {
                command[*index_write] += REG;
                (*index_write)++;
                command[*index_write] = REGISTERS[j].index;
                break;
            }
        }
    }
 
    if (type_arg == TYPE_ARG_LAB) {
        int was_label = 0;
        command[*index_write] += NUM;
        (*index_write)++;
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strcmp(argument, LABELS[j].name) == 0) {
                command[*index_write] = LABELS[j].index;
                was_label = 1;
                break;
            }
        }
        if (!was_label) {
            command[*index_write] = POISON_LABEL;
            pointers_labels[*number_fixup] = (Pointers_label) { number_string, (*index_write), 
                                                                len_arg, (*number_char) + count_arg_spaces};
            (*number_fixup)++;
        }
    }
    free(argument);
    return GLOBAL_ERROR_NO;
}

static Type_arg check_type_arg(const char *arg)
{
    const int len_arg = (int)strlen(arg);

    if (arg[0] == '[' && arg[len_arg - 1] == ']') return TYPE_ARG_MEM;

    if (arg[len_arg - 1] == ':') return TYPE_ARG_LAB;

    for (int j = 0; j < COUNT_REGISTERS; j++) {
        if (strcmp(arg, REGISTERS[j].name) == 0)
            return TYPE_ARG_REG;
    }
    return TYPE_ARG_NUM;
}

static GlobalErrors read_len_arg(int *count_spaces, int *len_arg, char *str)
{
    while (str[*count_spaces] == ' ')
        (*count_spaces)++;

    if (str[*count_spaces] == '[') {

        (*len_arg)++;
        while (str[*count_spaces + (*len_arg)] == ' ')
            (*len_arg)++;

        while (str[*count_spaces + (*len_arg)] != ' ')
            (*len_arg)++;

        while (str[*count_spaces + (*len_arg)] == ' ')
            (*len_arg)++;

        if (str[*count_spaces + (*len_arg)] != ']')
            return GLOBAL_ERROR_INPUT_FILE;

        (*len_arg)++;

        str[(*count_spaces) + (*len_arg)] = '\0';

        return GLOBAL_ERROR_NO;
    }

    while(str[(*count_spaces) + (*len_arg)] != '\n' &&
          str[(*count_spaces) + (*len_arg)] != '\0' &&
          str[(*count_spaces) + (*len_arg)] != ' ')
            (*len_arg)++;

    str[(*count_spaces) + (*len_arg)] = '\0';
    return GLOBAL_ERROR_NO;
}

static GlobalErrors get_spaces_lr(int *l_spaces, int *r_spaces, const char *str)
{
    while (str[*l_spaces] == ' ')
        (*l_spaces)++;

    int len = 0;
    while (str[*l_spaces + len] != ']' && str[*l_spaces + len] != ' ')
        len++;
    
    while (str[*l_spaces] == ' ')
        (*r_spaces)++;

    return GLOBAL_ERROR_NO;
}