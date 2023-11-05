#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <cctype>

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

static GlobalErrors get_arg(const DATA *src, int *command, int *index_write, 
                            int *number_fixup, int number_string, 
                            Pointers_label *pointers_labels, FILE *listing);

static Type_arg check_type_arg(char *arg);
static char *skip_spaces(char *s);
static char *skip_word(char *s);
static char *get_word(char *s);
static int is_label(char *s);
static int is_memory(char *s);
static int is_register(char *s);
static void check_len(char *s, int *len);
static int check_empty(char *string);
static GlobalErrors read_len_arg(int *count_spaces, int *len_arg, char *str);
static void fixup_lables(int number_fixup, const DATA *src, Pointers_label *pointers_labels, int **command);
static GlobalErrors get_spaces_lr(int *l_spaces, int *r_spaces, const char *str);

#define NUN 0
#define REG (1 << 5)
#define NUM (1 << 6)
#define MEM (1 << 7)

#define DEF_CMD(name_cmd, num, type_args, args, code)                                       \
    if (strncmp(src->pointers[number_string], #name_cmd, len_command) == 0)                 \
    {                                                                                       \
        fprintf(listing, "%s ", #name_cmd);                                                 \
        command[index_write] = num;                                                         \
        src->pointers[number_string] = skip_word(src->pointers[number_string]);             \
        src->pointers[number_string] = skip_spaces(src->pointers[number_string]);           \
        if (args > 0) {                                                                     \
            error = get_arg(src, command, &index_write, &number_fixup,                      \
                            number_string, pointers_labels, listing);                       \
            if (error) return error;                                                        \
            fprintf(listing, "%d %d\t", num, command[index_write]);                         \
        } else {                                                                            \
            fprintf(listing, "-\t%d -\t", num);                                             \
        }                                                                                   \
        index_write++;                                                                      \
    }                                                                                       \
    else

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, FILE *listing)
{
    GlobalErrors error = GLOBAL_ERROR_NO;
    if (!dest) return GLOBAL_ERROR_READ_FILE;

    fprintf(listing, "ind  num  com\targ\tbin_com\tbin_arg\n");

    int *command = (int *)calloc(2 * (src->commands_count + MAGIC_INTS), sizeof(int));
    if (!command)
        return GLOBAL_ERROR_READ_FILE;

    command = (int *)memcpy(command, MAGIC_DATA, MAGIC_BYTES);
    if (!command)
        return GLOBAL_ERROR_READ_FILE;

    Pointers_label pointers_labels[MAX_COUNT_LABELS] = {};

    int number_lable = 0;
    int number_string = 0;
    int index_write = MAGIC_INTS;
    int number_fixup = 0;
    while (number_string < src->commands_count) {

        char *comment = strchr(src->pointers[number_string], '/');
        if (comment) 
            comment[0] = '\0';

        if (check_empty(src->pointers[number_string]))
        {
            number_string++;
            continue;
        }
        fprintf(listing, "%.4d ", index_write - MAGIC_INTS);
        fprintf(listing, "%.4d ", number_string);
        
        src->pointers[number_string] = skip_spaces(src->pointers[number_string]);

        int len_command = 0;
        check_len(src->pointers[number_string], &len_command);

        #include "../Codegen.inc.h"
        {
            char *label = src->pointers[number_string];
            int len_lable = 0, count_arg_spaces = 0;
            error = read_len_arg(&count_arg_spaces, &len_lable, label);
            if (error) return error;

            if (label[len_lable - 1] == ':') {
                LABELS[number_lable] = (Label){label, index_write};
                fprintf(listing, "%s\t", label);
                fprintf(labels, "%s\n%d\n", label, index_write);
                number_lable++;
            }
        }
        fprintf(listing, "\n");
        number_string++;
    }
    fixup_lables(number_fixup, src, pointers_labels, &command);

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

static void fixup_lables(int number_fixup, const DATA *src, Pointers_label *pointers_labels, int **command)
{
    printf("FIXUP:\n");
    printf("Need count fixups: %d\n", number_fixup);
    for (int i = 0; i < number_fixup; i++) {
        char *st = src->pointers[pointers_labels[i].in_src];
        st = skip_spaces(st);
        int len_label = 0;
        check_len(st, &len_label);
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strncmp(st, LABELS[j].name, len_label) == 0) {
                (*command)[pointers_labels[i].in_bin] = LABELS[j].index;
                printf("Fixed: %s %d\n", LABELS[j].name, LABELS[j].index);
                break;
            }
        }
    }
}

static GlobalErrors get_arg(const DATA *src, int *command, int *index_write, 
                            int *number_fixup, int number_string, 
                            Pointers_label *pointers_labels, FILE *listing)
{
    GlobalErrors error = GLOBAL_ERROR_NO;

    int int_arg = 0;
    int count_read = sscanf(src->pointers[number_string], "%d", &int_arg);
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
    error = read_len_arg(&count_arg_spaces, &len_arg, src->pointers[number_string]);
    if(error) return error;

    argument = (char *)realloc(argument, len_arg * sizeof(char));
    if (!argument)
        return GLOBAL_ERROR_ALLOC_FAIL;

    memcpy(argument, &src->pointers[number_string][count_arg_spaces], len_arg);
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
            pointers_labels[*number_fixup] = (Pointers_label) { number_string, (*index_write) };
            (*number_fixup)++;
        }
    }
    free(argument);
    return GLOBAL_ERROR_NO;
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

static Type_arg check_type_arg(char *arg)
{
    arg = skip_spaces(arg);
    if (is_memory(arg))     return TYPE_ARG_MEM;
    if (is_label(arg))      return TYPE_ARG_LAB;
    if (is_register(arg))   return TYPE_ARG_REG;
    return TYPE_ARG_NUM;
}

static int is_label(char *s)
{
    char *old = s;
    s = skip_word(s);
    if (s[-1] == ':') { s = old; return 1; }
    s = old;
    return 0;
}

static int is_memory(char *s)
{
    char *old = s;
    if (s[0] != '[') { s = old; return 0; }
    s = skip_spaces(&s[1]);
    s = skip_word(s);
    s = skip_spaces(s);
    if (s[0] != ']') { s = old; return 0; }
    s = old;
    return 1;
}

static int is_register(char *s)
{
    char *old = s;
    for (int j = 0; j < COUNT_REGISTERS; j++) {
        if (strcmp(s, REGISTERS[j].name) == 0) {
            s = old; 
            return 1; 
        }
    }
    s = old;
    return 0;
}

static char *skip_spaces(char *s)
{
    int i = 0;
    while (s[i] == ' ')
        i++;
    return &s[i];
}

static void check_len(char *s, int *len)
{
    char *old = s;
    while (s[*len] != '\0' && !isspace(s[*len]))
        (*len)++;
    s = old;
}

static char *skip_word(char *s)
{
    int i = 0;
    while (s[i] != '\0' && !isspace(s[i]))
        i++;
    return &s[i];
}

static int check_empty(char *string)
{
    if(*skip_spaces(string) == '\0')    return 1;
    else                                return 0;
}