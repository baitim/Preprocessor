#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Stack/Stack.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));
    
    calculate("files/Asm.bin");

    printf(print_lblue("\nBye\n"));

    return 0;
}
