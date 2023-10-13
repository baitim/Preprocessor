#include <stdlib.h>
#include <string.h>

#include "Asm.h"

Errors process_input_commands_txt(FILE *dest, const Data *src)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    int number_string = 0;
    while (number_string < src->commands_count) {
        for (int i = 0; i < COUNT_COMMANDS; i++) {
            if (strcmp(COMMANDS[i].name, src->pointers[number_string]) == 0) {
                fprintf(dest, "%d", COMMANDS[i].opcode);

                if (COMMANDS[i].arg_types > 0) {
                    number_string++;

                    bool is_reg = false;
                    for (int j = 0; j < COUNT_REGISTERS; j++) {
                        if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {
                            fprintf(dest, " %d", REGISTERS[j].index);
                            is_reg = true;
                            break;
                        }
                    }

                    if (!is_reg) 
                        fprintf(dest, " %d", atoi(src->pointers[number_string]));

                }
                
                break;
            }
        }
        number_string++;
        fprintf(dest, "\n");
    }
    return error;
}

Errors process_input_commands_bin(FILE *dest, const Data *src)
{
    if (!dest) return ERROR_READ_FILE;
    Errors error = ERROR_NO;

    char *command = (char *)calloc(src->commands_count * sizeof(int), sizeof(char));
    if (!command)
        return ERROR_READ_FILE;

    int number_string = 0;
    while (number_string < src->commands_count) {
        for (int i = 0; i < COUNT_COMMANDS; i++) {
            if (strcmp(COMMANDS[i].name, src->pointers[number_string]) == 0) {
                *((int *)command + number_string) = COMMANDS[i].opcode;
                number_string++;

                if (COMMANDS[i].arg_types > 0) {
                    bool is_reg = false;
                    for (int j = 0; j < COUNT_REGISTERS; j++) {
                        if (strcmp(src->pointers[number_string], REGISTERS[j].name) == 0) {
                            *((int *)command + number_string) = REGISTERS[j].index;
                            is_reg = true;
                            break;
                        }
                    }

                    if (!is_reg) 
                        *((int *)command + number_string) = atoi(src->pointers[number_string]);
                    
                    number_string++;
                }
                break;
            }
        }
    }
    int count_print = (int)fwrite(command, sizeof(command[0]), number_string * sizeof(int), dest);
    if (count_print != src->commands_count * (int)sizeof(int)) {
        free(command);
        return ERROR_WRITE_FILE;
    }
    free(command);
    return error;
}