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

GlobalErrors process_input_commands_bin(FILE *dest, const DATA *src, FILE *labels, FILE *listing);

#endif // TO_BYTE_CODE_H