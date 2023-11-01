#include <assert.h>
#include <stdlib.h>

#include "Input.h"
#include "Asm.h"
#include "../Labels.h"
#include "../ANSI_colors.h"
#include "../Output.h"
#include "../Process_cmd.h"

int main(int argc, const char *argv[])
{
    int error = GLOBAL_ERROR_NO;

    CMD_INPUT_DATA cmd_data = { };
    input_cmd(argc, argv, &cmd_data);

    if (cmd_data.is_help)
        print_help();

    if (!cmd_data.is_input_txt || !cmd_data.is_asm_bin 
        || !cmd_data.is_labels || !cmd_data.is_listing
        || !cmd_data.input_txt || !cmd_data.asm_bin 
        || !cmd_data.labels    || !cmd_data.listing) {
        fprintf(stderr, print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 1; 
    }

    DATA src = {};
    error = create_data(&src, cmd_data.input_txt);
    if (error) {
        dump(error);
        return 1;
    }

    FILE *dest = fopen(cmd_data.asm_bin, "wb");
    if (!dest) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }

    FILE *labels = fopen(cmd_data.labels, "w"); 
    if (!labels) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }

    FILE *listing = fopen(cmd_data.listing, "w"); 
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

    //dump_labels();

    error = process_fixup(&src, cmd_data.asm_bin, pointers_labels, count_fixup);
    if (error) {
        dump(error);
        return 1;
    }

    dtor_data(&src);
    fclose(labels);
    return 0;
}
