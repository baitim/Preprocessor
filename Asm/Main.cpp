#include <assert.h>
#include <stdlib.h>

#include "Input.h"
#include "Asm.h"
#include "../Labels.h"
#include "../ANSI_colors.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 4;

    if (argc < COUNT_FROM_CMD) {
        fprintf(stderr, print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0; 
    }

    Data src = {};
    create_data(&src, argv[1]);
    // ??

    printf("count commands = %d\n", src.commands_count);
    for (int i = 0; i < src.commands_count; i++)
        puts(src.pointers[i]);
    puts("---------------");

    FILE *dest = fopen(argv[2], "wb");
    assert(dest);
    //????
    FILE *labels = fopen(argv[3], "w"); 
    assert(labels);
    //????

    int count_fixup = 0;
    Pointers_label pointers_labels[MAX_COUNT_LABELS] = {};

    if (process_input_commands_bin(dest, &src, labels, pointers_labels, &count_fixup)) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0;
    }
    fclose(dest);

    if (process_fixup(&src, argv[2], pointers_labels, count_fixup)) {
        printf(print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 0;
    }

    dtor_data(&src);
    fclose(labels);
    return 0;
}
