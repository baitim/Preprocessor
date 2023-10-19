#include "Input.h"
#include "Asm.h"
#include "../ANSI_colors.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 4;

    if (argc < COUNT_FROM_CMD) {
        printf (print_lred("CMD INPUT ERROR\n"));
        return 0; 
    }

    Data src = {};
    create_data(&src, argv[1]);

    FILE *dest = fopen(argv[2], "wb");
    assert(dest);
    FILE *labels = fopen(argv[3], "w"); 
    assert(labels);

    int count_fixup = 0;
    int *fixup = (int *)calloc(MAX_COUNT_LABELS, sizeof(int));

    if (process_input_commands_bin(dest, &src, labels, fixup, &count_fixup)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }
    fclose(dest);

    if (process_fixup(&src, argv[2], fixup, count_fixup)) {
        printf (print_lred("ERROR\n"));
        return 0;
    }

    dtor_data(&src);
    fclose(labels);
    return 0;
}
