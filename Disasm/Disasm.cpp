#include <stdlib.h>
#include <string.h>

#include "../Asm/Input.h"
#include "Disasm.h"

#define DEF_CMD(name_cmd, num, args, code)                                      \
    if (int_instruct == CMD_ ## name_cmd) {                                     \
        fprintf(dest, "%s", #name_cmd);                                         \
        if (args == 0) {                                                        \
            fprintf(dest, "\n");                                                \
            continue;                                                           \
        }                                                                       \
        if (args > 0) {                                                         \
            int value = 0;                                                      \
            count_input = fscanf(src, "%d", &value);                            \
            if (count_input != 1)                                               \
                return ERROR_READ_FILE;                                         \
                                                                                \
            int is_reg = 0;                                                     \
            for (int j = 0; j < COUNT_REGISTERS; j++) {                         \
                if (value == REGISTERS[j].index) {                              \
                    fprintf(dest, " %s", REGISTERS[j].name);                    \
                    is_reg = 1;                                                 \
                    break;                                                      \
                }                                                               \
            }                                                                   \
            if (!is_reg)                                                        \
                fprintf(dest, " %d", value);                                    \
        }                                                                       \
        fprintf(dest, "\n");                                                    \                                                                      
        continue;                                                               \
    }                                                                           \                                                                              
    else
Errors process_byte_commands_txt(FILE *dest, FILE *src)
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

        #include "../DSL"
        {;}

        fprintf(dest, "\n");
    }

    free(instruction);
    return error;
}
#undef DEF_CMD

#define DEF_CMD(name_cmd, num, args, code)                                      \
    if (int_instruct == CMD_ ## name_cmd) {                                     \
        fprintf(dest, "%s", #name_cmd);                                         \
        if (args == 0) {                                                        \
            fprintf(dest, "\n");                                                \
            continue;                                                           \
        }                                                                       \
        if (args > 0) {                                                         \
            int value = 0;                                                      \
            int_instruct = *((int *)command + number_command);                  \
            number_command++;                                                   \
                                                                                \
            int is_reg = 0;                                                     \
            for (int j = 0; j < COUNT_REGISTERS; j++) {                         \
                if (value == REGISTERS[j].index) {                              \
                    fprintf(dest, " %s", REGISTERS[j].name);                    \
                    is_reg = 1;                                                 \
                    break;                                                      \
                }                                                               \
            }                                                                   \
            if (!is_reg)                                                        \
                fprintf(dest, " %d", value);                                    \
        }                                                                       \
        fprintf(dest, "\n");                                                    \                                                                      
        continue;                                                               \
    }                                                                           \                                                                              
    else
Errors process_byte_commands_bin(FILE *dest, const char *str_src)
{
    FILE *src =  fopen(str_src, "rb");
    if (!src) return ERROR_READ_FILE;
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    const int size_file = (int)fsize(str_src);
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
        
        fprintf(dest, "\n");
    }

    free(command);
    return error;
}
#undef DEF_CMD