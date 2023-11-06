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
static int is_label(char *s);
static int is_memory(char *s);
static int is_register(char *s);
static void check_len(char *s, int *len);
static int check_empty(char *string);
static void fixup_lables(int number_fixup, const DATA *src, Pointers_label *pointers_labels, int **command);
static int get_number(const DATA *src, int number_string, int *command, int *index_write, FILE *listing);
static int get_memory(const DATA *src, int number_string, int *command, int *index_write);
static int get_label(const DATA *src, int number_string, int *command, int *index_write, 
                     Pointers_label *pointers_labels, int *number_fixup);
static int get_register(const DATA *src, int number_string, int *command, int *index_write);
static int parse_memory(char **dest, char *src);
static void make_end_null(char *str);

#define NUN 0
#define REG (1 << 5)
#define NUM (1 << 6)
#define MEM (1 << 7)

#define DEF_CMD(name_cmd, num, type_args, args, code)                                       \
    if (strncmp(src->pointers[number_string], #name_cmd, len_command) == 0)                 \
    {                                                                                       \
        fprintf(listing, " %*s ", 7, #name_cmd);                                            \
        command[index_write] = num;                                                         \
        src->pointers[number_string] = skip_word(src->pointers[number_string]);             \
        src->pointers[number_string] = skip_spaces(src->pointers[number_string]);           \
        if (args > 0) {                                                                     \
            error = get_arg(src, command, &index_write, &number_fixup,                      \
                            number_string, pointers_labels, listing);                       \
            if (error) return error;                                                        \
            fprintf(listing, "  |  %d %d\t", num, command[index_write]);                    \
        } else {                                                                            \
            fprintf(listing, "%*c-  |  %d -\t", 20, ' ', num);                             \
        }                                                                                   \
        index_write++;                                                                      \
    }                                                                                       \
    else

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, FILE *listing)
{
    GlobalErrors error = GLOBAL_ERROR_NO;
    if (!dest) return GLOBAL_ERROR_READ_FILE;

    fprintf(listing, " ind  | num  | src                            | bin\n");

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
        fprintf(listing, " %.4d | %.4d |", index_write - MAGIC_INTS, number_string);
        
        src->pointers[number_string] = skip_spaces(src->pointers[number_string]);

        int len_command = 0;
        check_len(src->pointers[number_string], &len_command);

        #include "../Codegen.inc.h"
        {
            char *label = src->pointers[number_string];
            int len_lable = 0;
            check_len(label, &len_lable);

            if (label[len_lable - 1] == ':') {
                LABELS[number_lable] = (Label){label, index_write};
                fprintf(listing, " %s", label);
                fprintf(labels, "%s\n%d\n", label, index_write);
                number_lable++;
                if (number_lable >= MAX_COUNT_LABELS)
                    return GLOBAL_ERROR_MAX_COUNT_LABELS;
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
    // Нет проверки на сигнатуру.
    if (get_number(src, number_string, command, index_write, listing) == 1)
        return GLOBAL_ERROR_NO;

    make_end_null(src->pointers[number_string]);
    fprintf(listing, "%*s", 21, src->pointers[number_string]);
    int type_arg = check_type_arg(src->pointers[number_string]);
 
    if (type_arg == TYPE_ARG_MEM) {
        if (get_memory(src, number_string, command, index_write) == 1)
            return GLOBAL_ERROR_NO;
        else
            return GLOBAL_ERROR_READ_FILE;
    }

    if (type_arg == TYPE_ARG_REG) {
        if (get_register(src, number_string, command, index_write) == 1)
            return GLOBAL_ERROR_NO;
        else 
            return GLOBAL_ERROR_READ_FILE;
    }
 
    if (type_arg == TYPE_ARG_LAB) {
        if (get_label(src, number_string, command, index_write, pointers_labels, number_fixup) == 1)
            return GLOBAL_ERROR_NO;
        else 
            return GLOBAL_ERROR_READ_FILE;
    }

    return GLOBAL_ERROR_NO;
}

static int get_number(const DATA *src, int number_string, int *command, int *index_write, FILE *listing)
{
    int int_arg = 0;
    int count_read = sscanf(src->pointers[number_string], "%d", &int_arg);
    if (count_read == 1) {
        command[*index_write] += NUM;
        (*index_write)++;
        command[*index_write] = int_arg;
        fprintf(listing, "%*d", 21, int_arg);
        return 1;
    }
    return 0;
}

static int get_memory(const DATA *src, int number_string, int *command, int *index_write)
{
    command[*index_write] += MEM;
    char *memory_arg = nullptr;
    if (parse_memory(&memory_arg, src->pointers[number_string]) != 1)
        return 0;

    int type_mem_arg = check_type_arg(memory_arg);

    if (type_mem_arg == TYPE_ARG_REG) {
        for (int j = 0; j < COUNT_REGISTERS; j++) {
            if (strcmp(memory_arg, REGISTERS[j].name) == 0) {
                command[*index_write] += REG;
                (*index_write)++;
                command[*index_write] = REGISTERS[j].index;
                break;                                                              
            }
        }
        return 1;
    }
    if (type_mem_arg == TYPE_ARG_NUM) {
        command[*index_write] += NUM;
        (*index_write)++;
        command[*index_write] = atoi(memory_arg);
        return 1;
    }
    return 0;
}

static int parse_memory(char **dest, char *src)
{
    if (src[0] != '[')
        return 0;
    src++;
    src = skip_spaces(src);
    *dest = src;
    src = skip_word(src);
    if (src[-1] == ']') {
        src[-1] = '\0';
        return 1;
    }
    src[0] = '\0';
    src++;
    src = skip_spaces(src);
    if (src[0] != ']')
        return 0;
    return 1;
}

static int get_label(const DATA *src, int number_string, int *command, int *index_write, 
                     Pointers_label *pointers_labels, int *number_fixup)
{
    int was_label = 0;
    command[*index_write] += NUM;
    (*index_write)++;
    for (int j = 0; j < MAX_COUNT_LABELS; j++) {
        if (!LABELS[j].name) break;
        if (strcmp(src->pointers[number_string], LABELS[j].name) == 0) {
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
    return 1;
}

static int get_register(const DATA *src, int number_string, int *command, int *index_write)
{
    for (int j = 0; j < COUNT_REGISTERS; j++) {
        if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {
            command[*index_write] += REG;
            (*index_write)++;
            command[*index_write] = REGISTERS[j].index;
            return 1;
        }
    }
    return 0;
}

static Type_arg check_type_arg(char *arg)
{
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
    if (s[-1] == ']') { s = old; return 1; }
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
    while (isspace(s[i]))
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

static void make_end_null(char *str)
{
    char *string = str;
    if (string[0] == '[') {
        string++;
        string = skip_spaces(string);
        string = skip_word(string);
        if (string[-1] == ']') {
            string[0] = '\0'; 
        } else {
            string = skip_spaces(string); 
            string[1] = '\0';
        }
    } else {
        string = skip_word(string);
        string[0] = '\0';
    }
}