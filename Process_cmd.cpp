#include "Process_cmd.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_cmd(int argc, const char *argv[], CMD_INPUT_DATA *cmd_data)
{
    assert(argv);
    assert(cmd_data);

    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < count_options; j++) {
        	if (strcmp(argv[i], OPTIONS[j].name) == 0) {
                (*OPTIONS[j].callback)(&argv[i], cmd_data);
                i += OPTIONS[j].n_args;
            }

        }
    }
}

void coef_input_txt_callback(const char* argv[], CMD_INPUT_DATA* data)
{
    data->is_input_txt = 1;
	data->input_txt = (char *) calloc(MAX_CMD_COMMAND_SIZE, sizeof(char));
	strcpy(data->input_txt, argv[1]);
}

void coef_asm_bin_callback(const char* argv[], CMD_INPUT_DATA* data)
{
    data->is_asm_bin = 1;
	data->asm_bin = (char *) calloc(MAX_CMD_COMMAND_SIZE, sizeof(char));
	strcpy(data->asm_bin, argv[1]);
}

void coef_disasm_txt_callback(const char* argv[], CMD_INPUT_DATA* data)
{
    data->is_disasm_txt = 1;
	data->disasm_txt = (char *) calloc(MAX_CMD_COMMAND_SIZE, sizeof(char));
	strcpy(data->disasm_txt, argv[1]);
}

void coef_labels_callback(const char* argv[], CMD_INPUT_DATA* data)
{
    data->is_labels = 1;
	data->labels = (char *) calloc(MAX_CMD_COMMAND_SIZE, sizeof(char));
	strcpy(data->labels, argv[1]);
}

void coef_listing_callback(const char* argv[], CMD_INPUT_DATA* data)
{
	data->is_listing = 1;
	data->listing = (char *) calloc(MAX_CMD_COMMAND_SIZE, sizeof(char));
	strcpy(data->listing, argv[1]);
}

void help_callback(const char* /*argv*/[], CMD_INPUT_DATA* data)
{
    data->is_help = 1;
}
