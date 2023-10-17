#include "Input.h"
#include "Asm.h"
#include "../ANSI_colors.h"

int main(int argc, const char *argv[])
{
    if (argc < 4) {
        printf (print_lred("CMD INPUT ERROR\n"));
        return 0; 
    }

    Data src = {};
    create_data(&src, argv[1]);
    FILE *dest = fopen(argv[2], "w");
    assert(dest);
    if (process_input_commands_txt(dest, &src)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }

    dest = fopen(argv[3], "wb");
    assert(dest);
    if (process_input_commands_bin(dest, &src)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }

    dtor_data(&src);
    fclose(dest);
    return 0;
}
