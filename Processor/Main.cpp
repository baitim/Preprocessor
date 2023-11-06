#include <stdio.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Debug.h"
#include "../Asm/Input.h"
#include "../Output.h"
#include "../Process_cmd.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of Processor.\n"
                       "# (c) BAIDIUSENOV TIMUR, 2023\n\n"));

    CMD_INPUT_DATA cmd_data = { };
    input_cmd(argc, argv, &cmd_data);

    if (cmd_data.is_help)
        print_help();

    if (!cmd_data.is_asm_bin || !cmd_data.asm_bin) {
        fprintf(stderr, print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 1; 
    }

    GlobalErrors error = calculate(cmd_data.asm_bin);
    if (error) {
        dump(error);
        return 1;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}
