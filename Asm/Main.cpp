#include "Input.h"
#include "Asm.h"
#include "../ANSI_colors.h"

int main()
{
    Data src = {};
    create_data(&src, "files/Input_commands.txt");
    FILE *dest = fopen("files/Asm.txt", "w");
    assert(dest);
    if (process_input_commands_txt(dest, &src)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }

    dest = fopen("files/Asm.bin", "wb");
    assert(dest);
    if (process_input_commands_bin(dest, &src)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }

    dtor_data(&src);
    fclose(dest);
    return 0;
}
