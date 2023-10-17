#include <assert.h>

#include "Disasm.h"
#include "../ANSI_colors.h"
#include "../Asm/Input.h"

int main(int argc, const char *argv[])
{
    if (argc < 5) {
        printf (print_lred("CMD INPUT ERROR\n"));
        return 0; 
    }

    FILE *src =  fopen(argv[1], "r");
    assert(src);
    FILE *dest = fopen(argv[2], "w");
    assert(dest);
    process_byte_commands_txt(dest, src);

    src =  fopen(argv[3], "rb");
    assert(src);
    dest = fopen(argv[4], "wb");
    assert(dest);
    const int size_file = (int)fsize(argv[3]);
    process_byte_commands_bin(dest, src, size_file);

    fclose(src);
    fclose(dest);
    return 0;
}