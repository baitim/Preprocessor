#ifndef PROCESS_CMD_H
#define PROCESS_CMD_H

#define MAX_CMD_COMMAND_SIZE 100

struct CMD_INPUT_DATA {
    int is_input_txt;
    int is_asm_bin;
    int is_disasm_txt;
    int is_labels;
    int is_listing;
    int is_help;
    char *input_txt;
    char *asm_bin;
    char *disasm_txt;
    char *labels;
    char *listing;
};

const int count_options = 6;

struct CMD_LINE_OPTION {
    const char *name;
    const char *description;
    int n_args;
    void (*callback)(const char* argv[], CMD_INPUT_DATA* data);
};

void input_cmd(int argc, const char *argv[], CMD_INPUT_DATA *cmd_data);

void coef_input_txt_callback(const char* argv[], CMD_INPUT_DATA* data);

void coef_asm_bin_callback(const char* argv[], CMD_INPUT_DATA* data);

void coef_disasm_txt_callback(const char* argv[], CMD_INPUT_DATA* data);

void coef_labels_callback(const char* argv[], CMD_INPUT_DATA* data);

void coef_listing_callback(const char* argv[], CMD_INPUT_DATA* data);

void help_callback(const char* argv[], CMD_INPUT_DATA* data);

const CMD_LINE_OPTION OPTIONS[] = {
    {"--input_txt",     "include src file",             1, coef_input_txt_callback},
    {"--asm_bin",       "include file for asm",         1, coef_asm_bin_callback},
    {"--disasm_txt",    "include file for disasm",      1, coef_disasm_txt_callback},
    {"--labels",        "include file for labels",      1, coef_labels_callback},
    {"--listing",       "include file for listing",     1, coef_listing_callback},
    {"--help",          "help",                         0, help_callback}
};
const int COUNT_OPTIONS = sizeof(OPTIONS) / sizeof(CMD_LINE_OPTION);

void print_help();

#endif // PROCESS_CMD_H