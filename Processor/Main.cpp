#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Stack/Stack.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));
    
    const int COUNT_FROM_CMD = 2;

    if (argc < COUNT_FROM_CMD) {
        printf (print_lred("CMD INPUT ERROR\n"));
        return 0; 
    }

    calculate(argv[1]);

    printf(print_lblue("\nBye\n"));

    return 0;
}
