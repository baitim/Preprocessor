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
    TYPE_ARG_MEM = 4,
    TYPE_ARG_ERR = -1,
};

static GlobalErrors process_asm_line(char *line, int *index_write, int number_string, 
                                     int *number_lable, FILE *listing, FILE *labels, 
                                     int *command, int *number_fixup,
                                     Pointers_label *pointers_labels);

static GlobalErrors get_arg(char *line, int *command, int *index_write, 
                            int number_string, int *number_fixup, int sgnt,
                            Pointers_label *pointers_labels, FILE *listing);

static Type_arg check_type_arg(char *arg, int sgnt);
static char *skip_spaces(char *s);
static char *skip_word(char *s);
static int is_label(char *s);
static int is_memory(char *s);
static int is_register(char *s);
static void check_len(char *s, int *len);
static int check_empty(char *string);
static void fixup_lables(int number_fixup, const DATA *src, Pointers_label *pointers_labels, int *command);
static int get_number(char *line, int *command, int *index_write, FILE *listing);
static int get_memory(char *line, int *command, int *index_write);
static int get_label(char *line, int number_string, int *command, int *index_write, 
                     Pointers_label *pointers_labels, int *number_fixup);
static int get_register(char *line, int *command, int *index_write);
static int parse_memory(char **dest, char *src);
static void make_end_null(char *str);

#define NUN 0
#define REG (1 << 5)
#define NUM (1 << 6)
#define MEM (1 << 7)
#define STR (1 << 8)

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, FILE *listing)
{
    if (!dest) return GLOBAL_ERROR_READ_FILE;

    GlobalErrors err = GLOBAL_ERROR_NO;
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

        err = process_asm_line(src->pointers[number_string], &index_write, number_string, 
                               &number_lable, listing, labels, command, &number_fixup, 
                               pointers_labels);

        if (err) return err;

        number_string++;
    }

    fixup_lables(number_fixup, src, pointers_labels, command);

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

static void fixup_lables(int number_fixup, const DATA *src, Pointers_label *pointers_labels, int *command)
{
    printf("FIXUP:\n");
    printf("Need count fixups: %d\n", number_fixup);
    for (int i = 0; i < number_fixup; i++) {
        char *st = src->pointers[pointers_labels[i].in_src];
        st = skip_spaces(st);
        st = skip_word(st);
        st = skip_spaces(st);
        int len_label = 0;
        check_len(st, &len_label);
        for (int j = 0; j < MAX_COUNT_LABELS; j++) {
            if (!LABELS[j].name) break;
            if (strncmp(st, LABELS[j].name, len_label) == 0) {
                command[pointers_labels[i].in_bin] = LABELS[j].index;
                printf("Fixed: %s %d\n", LABELS[j].name, LABELS[j].index);
                break;
            }
        }
    }
}

#define DEF_CMD(name_cmd, num, type_args, args, code)   \
    if (strncmp(line, #name_cmd, len_command) == 0)     \
    {                                                   \
        op_code = num;                                  \
        fprintf(listing, " %*s ", 7, #name_cmd);        \
        count_args = args;                              \
        sgnt = type_args;                               \
    }                                                   \
    else
static GlobalErrors process_asm_line(char *line, int *index_write, int number_string, 
                                     int *number_lable, FILE *listing, FILE *labels, 
                                     int *command, int *number_fixup, 
                                     Pointers_label *pointers_labels)
{
    GlobalErrors err = GLOBAL_ERROR_NO;

    char *comment = strchr(line, '/');
    if (comment) comment[0] = '\0';

    if (check_empty(line)) return GLOBAL_ERROR_NO;
    
    fprintf(listing, " %.4d | %.4d |", (*index_write) - MAGIC_INTS, number_string);
        
    line = skip_spaces(line);
    int len_command = 0;
    check_len(line, &len_command);

    int op_code = 0;
    int count_args = 0;
    int sgnt = 0;

    #include "../Codegen.inc.h"
    {
        char *label = line;
        int len_lable = 0;
        check_len(label, &len_lable);

        if (label[len_lable - 1] == ':') {
            LABELS[(*number_lable)] = (Label){label, (*index_write)};
            fprintf(listing, " %s", label);
            fprintf(labels, "%s\n%d\n", label, (*index_write));
            (*number_lable)++;
            if ((*number_lable) >= MAX_COUNT_LABELS - 1)
                return GLOBAL_ERROR_MAX_COUNT_LABELS;
        }
        fprintf(listing, "\n");
        return GLOBAL_ERROR_NO;
    }

    command[(*index_write)] = op_code;
    line = skip_word(line);

    if (count_args == 0) {
        fprintf(listing, "%*c-  |  %d -\t", 20, ' ', op_code);
        (*index_write)++;
    }

    for (int i = 0; i < count_args; i++) {
        line = skip_spaces(line);
        err = get_arg(line, command, index_write, number_string, number_fixup, 
                      sgnt, pointers_labels, listing);
        if (err) return err;

        fprintf(listing, "  |  %d %d\t", op_code, command[(*index_write)]);
        (*index_write)++;
    }

    fprintf(listing, "\n");
    return GLOBAL_ERROR_NO;
}
#undef DEF_CMD

static GlobalErrors get_arg(char *line, int *command, int *index_write,
                            int number_string, int *number_fixup, int sgnt, 
                            Pointers_label *pointers_labels, FILE *listing)
{
    if (get_number(line, command, index_write, listing) == 1)
        return GLOBAL_ERROR_NO;

    make_end_null(line);
    fprintf(listing, "%*s", 21, line);
    Type_arg type_arg = check_type_arg(line, sgnt);
    if (type_arg == TYPE_ARG_ERR)
        return GLOBAL_ERROR_READ_FILE;
 
    if (type_arg == TYPE_ARG_MEM) {
        if (get_memory(line, command, index_write) == 1)
            return GLOBAL_ERROR_NO;
        else
            return GLOBAL_ERROR_READ_FILE;
    }

    if (type_arg == TYPE_ARG_REG) {
        if (get_register(line, command, index_write) == 1)
            return GLOBAL_ERROR_NO;
        else 
            return GLOBAL_ERROR_READ_FILE;
    }
 
    if (type_arg == TYPE_ARG_LAB) {
        if (get_label(line, number_string, command, index_write, pointers_labels, number_fixup) == 1)
            return GLOBAL_ERROR_NO;
        else 
            return GLOBAL_ERROR_READ_FILE;
    }

    return GLOBAL_ERROR_NO;
}

static int get_number(char *line, int *command, int *index_write, FILE *listing)
{
    int int_arg = 0;
    int count_read = sscanf(line, "%d", &int_arg);
    if (count_read == 1) {
        command[*index_write] += NUM;
        (*index_write)++;
        command[*index_write] = int_arg;
        fprintf(listing, "%*d", 21, int_arg);
        return 1;
    }
    return 0;
}

static int get_memory(char *line, int *command, int *index_write)
{
    command[*index_write] += MEM;
    char *memory_arg = nullptr;
    if (parse_memory(&memory_arg, line) != 1)
        return 0;

    int valid_type = (NUM | REG);
    Type_arg type_mem_arg = check_type_arg(memory_arg, valid_type);
    if (type_mem_arg == TYPE_ARG_ERR)
        return GLOBAL_ERROR_READ_FILE;

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

static int get_label(char *line, int number_string, int *command, int *index_write, 
                     Pointers_label *pointers_labels, int *number_fixup)
{
    int was_label = 0;
    command[*index_write] += NUM;
    (*index_write)++;
    for (int j = 0; j < MAX_COUNT_LABELS; j++) {
        if (!LABELS[j].name) break;
        if (strcmp(line, LABELS[j].name) == 0) {
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

static int get_register(char *line, int *command, int *index_write)
{
    for (int j = 0; j < COUNT_REGISTERS; j++) {
        if (strcmp(line, REGISTERS[j].name) == 0) {
            command[*index_write] += REG;
            (*index_write)++;
            command[*index_write] = REGISTERS[j].index;
            return 1;
        }
    }
    return 0;
}

static Type_arg check_type_arg(char *arg, int sgnt)
{
    if (is_memory(arg) && (sgnt & MEM))     return TYPE_ARG_MEM;
    if (is_label(arg) && (sgnt & NUM))      return TYPE_ARG_LAB;
    if (is_register(arg) && (sgnt & REG))   return TYPE_ARG_REG;
    if (sgnt & NUM)                         return TYPE_ARG_NUM;
    return TYPE_ARG_ERR;
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