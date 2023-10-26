#include <assert.h>
#include <stdlib.h>

#include "Input.h"
#include "Asm.h"
#include "../Labels.h"
#include "../ANSI_colors.h"
#include "../Output.h"

int main(int argc, const char *argv[])
{
    const int COUNT_FROM_CMD = 5;
    int error = GLOBAL_ERROR_NO;

    if (argc < COUNT_FROM_CMD) {
        fprintf(stderr, print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 1; 
    }

    Data src = {};
    error = create_data(&src, argv[1]);
    if (error) {
        dump(error);
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (!dest) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }

    FILE *labels = fopen(argv[3], "w"); 
    if (!labels) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }

    FILE *listing = fopen(argv[4], "w"); 
    if (!listing) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }

    int count_fixup = 0;
    Pointers_label pointers_labels[MAX_COUNT_LABELS] = {};

    error = process_input_commands_bin(dest, &src, labels, pointers_labels, &count_fixup, listing);
    if (error) {
        dump(error);
        return 1;
    }
    fclose(dest);

    error = process_fixup(&src, argv[2], pointers_labels, count_fixup);
    if (error) {
        dump(error);
        return 1;
    }

    dtor_data(&src);
    fclose(labels);
    return 0;
}
