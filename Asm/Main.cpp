#include "Input.h"
#include "Asm.h"

int main()
{
    Data src = {};

    create_data(&src, "txt/Input_commands.txt");

    FILE *dest = fopen("txt/Asm.txt", "w");
    process_input_commands(dest, &src);

    return 0;
}
