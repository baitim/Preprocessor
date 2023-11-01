#include <assert.h>

#include "Disasm.h"
#include "../ANSI_colors.h"
#include "../Asm/Input.h"
#include "../Output.h"
#include "../Process_cmd.h"

int main(int argc, const char *argv[])
{
    CMD_INPUT_DATA cmd_data = { };
    input_cmd(argc, argv, &cmd_data);

    if (cmd_data.is_help)
        print_help();

    if (!cmd_data.is_asm_bin || !cmd_data.is_disasm_txt
        || !cmd_data.asm_bin || !cmd_data.disasm_txt) {
        fprintf(stderr, print_lred("ERROR in %s %s %d\n"), __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return 1; 
    }
    FILE *dest = fopen(cmd_data.disasm_txt, "wb");
    if (!dest) {
        dump(GLOBAL_ERROR_READ_FILE);
        return 1;
    }
    GlobalErrors error = process_byte_commands_bin(dest, cmd_data.disasm_txt);
    if (error) {
        dump(error);
        return 1;
    }
    
    fclose(dest);
    return 0;
}