#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ANSI_colors.h"
#include "Output.h"
#include "Config.h"

void print_stack(Stack *stack)
{
    if (stack && stack->data) {
        for (int i = 0; i < stack->size; i++) {
            printf(print_lcyan("i = %d "), i);
            print_el(&stack->data[i]);
        }
    }
}

void print_commands(const char *name_of_file, int number_command)
{
    FILE *src = fopen(name_of_file, "r");
    assert(src);

    char *instruction = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
    assert (instruction);

    printf("command: %d\n", number_command);

    while (true) {
        int count_input = fscanf(src, "%s", instruction);
        if (count_input != 1)
            break;

        printf("%s ", instruction);
    }

    printf("\n");
    fclose(src);
}