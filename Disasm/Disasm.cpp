#include <stdlib.h>
#include <string.h>

#include "Disasm.h"

static int powf(int x, int p);

#define DEF_CMD(name_cmd, num, type_args, args, code)                           \
    if (int_instruct % powf(2, FREE_BYTES)  == CMD_ ## name_cmd) {              \
        fprintf(dest, "%s", #name_cmd);                                         \
        if (args == 0) {                                                        \
            fprintf(dest, "\n");                                                \
            continue;                                                           \
        }                                                                       \
        for (int i = 0; i < args; i++) {                                        \
            int value  = *((int *)command + number_command);                    \
            number_command++;                                                   \
            if (int_instruct & (1 << REG)) {                                    \
                for (int j = 0; j < COUNT_REGISTERS; j++) {                     \
                    if (value == REGISTERS[j].index) {                          \
                        fprintf(dest, " %s", REGISTERS[j].name);                \
                        break;                                                  \
                    }                                                           \
                }                                                               \
            }                                                                   \
            if (int_instruct & (1 << LAB)) {                                    \
                for (int j = 0; j < MAX_COUNT_LABELS; j++) {                    \
                    if (value == LABELS[j].index) {                             \
                        fprintf(dest, " %s", LABELS[j].name);                   \
                        break;                                                  \
                    }                                                           \
                }                                                               \
            }                                                                   \
            if (int_instruct & (1 << NUM))                                      \
                fprintf(dest, " %d", value);                                    \
                                                                                \
            fprintf(dest, "\n");                                                \
        }                                                                       \
        continue;                                                               \
    }                                                                           \
    else
Errors process_byte_commands_bin(FILE *dest, const char *name_src)
{
    FILE *src =  fopen(name_src, "rb");
    if (!src) return ERROR_READ_FILE;
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    const int size_file = (int)fsize(name_src);

    char *command = (char *)calloc(size_file, sizeof(char));
    if (!command)
        return ERROR_ALLOC_FAIL;

    int count_read = (int)fread(command, sizeof(command[0]), size_file, src);
    if (count_read != size_file)
        return ERROR_READ_FILE;
                                                        
    int number_command = 0;
    while (number_command * (int)sizeof(int) < size_file) {
        int int_instruct = *((int *)command + number_command);
        number_command++;
        
        #include "../DSL"
        {;}
    }

    free(command);
    return error;
}
#undef DEF_CMD

Errors read_labels(Data *label_index)
{
    assert(label_index);
    
    int number_word = 0;
    int number_label = 0;
    while (number_word < label_index->commands_count - 1) {
        LABELS[number_label].name = label_index->pointers[number_word];
        number_word++;
        LABELS[number_label].index = atoi(label_index->pointers[number_word]);
        number_word++;
        number_label++;
    }
    fprintf(stderr, "END_FUNC\n");
}

static int powf(int x, int p)
{
    if (p == 0) return 1;
    if (p % 2 == 1) return x * powf(x, p - 1);
    int z = powf(x, p/2);
    return z * z;
}