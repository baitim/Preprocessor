#include "Disasm.h"

int main()
{
    FILE *src =  fopen("txt/Commands_in_byte_code.txt", "r");
    FILE *dest = fopen("txt/Commands_from_byte_code.txt", "w");
    process_byte_commands(dest, src);

    return 0;
}