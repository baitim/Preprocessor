#include <assert.h>

#include "Disasm.h"
#include "../ANSI_colors.h"
#include "../Asm/Input.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 5;

    if (argc < COUNT_FROM_CMD) {
        printf (print_lred("CMD INPUT ERROR\n"));
        return 0; 
    }

    FILE *src =  fopen(argv[1], "r");
    assert(src);
    FILE *dest = fopen(argv[2], "w");
    assert(dest);
    process_byte_commands_txt(dest, src);

    dest = fopen(argv[4], "wb");
    assert(dest);
    process_byte_commands_bin(dest, argv[3]);

    fclose(src);
    fclose(dest);
    return 0;
}