#include <assert.h>

#include "Disasm.h"
#include "../ANSI_colors.h"
#include "../Asm/Input.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 4;

    if (argc < COUNT_FROM_CMD) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0; 
    }

    Data label_index = {};
    create_data(&label_index, argv[3]);

    read_labels(&label_index);

    FILE *dest = fopen(argv[2], "wb");
    assert(dest);
    process_byte_commands_bin(dest, argv[1]);

    dtor_data(&label_index);
    fclose(dest);
    return 0;
}