#include <stdio.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Asm/Input.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));
    
    const int COUNT_FROM_CMD = 3;

    if (argc < COUNT_FROM_CMD) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0; 
    }

    Data label_index = {};
    create_data(&label_index, argv[2]);

    calculate(argv[1]);

    dtor_data(&label_index);

    printf(print_lblue("\nBye\n"));

    return 0;
}
