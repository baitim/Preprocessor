#ifndef TO_BYTE_CODE_H
#define TO_BYTE_CODE_H

#include <stdio.h>

#include "Input.h"
#include "../Errors.h"
#include "../Config.h"

Errors process_input_commands_bin(FILE *dest, const Data *src, FILE *labels, int *fixup, int *count_fixup);

Errors process_fixup(const Data *src, const char *bin_file, int *fixup, int count_fixup);

#endif // TO_BYTE_CODE_H