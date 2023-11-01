#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ANSI_colors.h"
#include "Output.h"
#include "Config.h"
#include "Process_cmd.h"

static void print_error(int error, const char *s);

static int powf(int x, int st);

void print_help()
{
    for (int i = 0; i < COUNT_OPTIONS; i++)
        printf(ANSI_LIGHT_GREEN "%s\t\t%s\n" ANSI_DEFAULT_COLOR, OPTIONS[i].name, OPTIONS[i].description);
}

void dump_(int err, const char *file, const char *func, int line)
{
    if (err == GLOBAL_ERROR_NO) {
        //fprintf(stderr, print_lgreen("dump: OK\n"));
        return;
    }

    fprintf(stderr, print_lred("ERROR: called from FILE = %s, FUNCTION = %s, LINE = %d\n"), 
                               file, func, line);

    for (int i = 1; i < COUNT_GLOBAL_ERRORS; i++) {
        if (err & powf(2, i - 1))
            print_error(err, GLOBAL_ERRORS[i].description);
    }
}

static void print_error(int error, const char *s)
{
    fprintf(stderr, print_lred("ERROR: %d %s\n"), error, s);
}

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

static int powf(int x, int st)
{
    if (st == 0) return 1;
    if (st % 2 == 1) return x * powf(x, st - 1);
    int z = powf(x, st / 2);
    return z * z;
}