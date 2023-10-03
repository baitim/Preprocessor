#include "To_byte_code.h"

#include <errno.h>

int main()
{
    FILE *src =  fopen("txt/Input_commands.txt", "r");
    FILE *dest = fopen("txt/Commands_in_byte_code.txt", "w");
    process_input_commands(dest, src);

    return 0;
}
