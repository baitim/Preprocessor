#include <stdio.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Asm/Input.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));
    
    const int COUNT_FROM_CMD = 2;

    if (argc < COUNT_FROM_CMD) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0; 
    }

    calculate(argv[1]);

    printf(print_lblue("\nBye\n"));

    return 0;
}
