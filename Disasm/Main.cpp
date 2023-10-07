#include "Disasm.h"

int main()
{
    FILE *src =  fopen("txt/Asm.txt", "r");
    FILE *dest = fopen("txt/Disasm.txt", "w");
    process_byte_commands(dest, src);

    return 0;
}