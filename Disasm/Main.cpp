#include <assert.h>

#include "Disasm.h"
#include "../ANSI_colors.h"
#include "../Asm/Input.h"
#include "../Output.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 3;

    if (argc < COUNT_FROM_CMD) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 1; 
    }

    FILE *dest = fopen(argv[2], "wb");
    if (!dest) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }
    GlobalErrors error = process_byte_commands_bin(dest, argv[1]);
    if (error) {
        dump(error);
        return 1;
    }
    
    fclose(dest);
    return 0;
}