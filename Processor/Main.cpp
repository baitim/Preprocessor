#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Stack/Stack.h"

int main()
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));
    
    /*if (stack_push(&stack, 9)) return 0;

    type_el value = {};
    if (stack_pop(&stack, &value)) return 0;
    printf(print_lcyan(type_el_print "\n"), el_print(value));
    */

    calculate("files/Asm.txt");

    printf(print_lblue("\nBye\n"));

    return 0;
}