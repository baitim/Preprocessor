#include <assert.h>

#include "Disasm.h"

int main()
{
    FILE *src =  fopen("files/Asm.txt", "r");
    assert(src);
    FILE *dest = fopen("files/Disasm_txt.txt", "w");
    assert(dest);
    process_byte_commands_txt(dest, src);

    dest = fopen("files/Disasm_bin.txt", "wb");
    assert(dest);
    process_byte_commands_bin(dest, "files/Asm.bin");

    fclose(src);
    fclose(dest);
    return 0;
}