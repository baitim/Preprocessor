#include "Input.h"
#include "Asm.h"

#include <errno.h>

int main()
{
    Data src = {};

    create_data(&src, "txt/Input_commands.txt");

    FILE *dest = fopen("txt/Commands_in_byte_code.txt", "w");
    process_input_commands(dest, &src);

    return 0;
}
