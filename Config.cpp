#include <stdio.h>

#include "Config.h"
#include "ANSI_colors.h"

void print_el(const type_el* elem)
{
    printf(print_lcyan("x = %d\n"), *elem);
}