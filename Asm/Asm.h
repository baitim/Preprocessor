#ifndef TO_BYTE_CODE_H
#define TO_BYTE_CODE_H

#include <stdio.h>

#include "Input.h"
#include "../Errors.h"
#include "../Config.h"

struct Pointers_label {
    int in_src;
    int in_bin;
    int len;
    int start;
};

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, 
                                        Pointers_label *pointers_labels, int *count_fixup, 
                                        FILE *listing);

GlobalErrors process_fixup(const DATA *src, const char *bin_file, 
                           Pointers_label *pointers_labels, int count_fixup);

#endif // TO_BYTE_CODE_H