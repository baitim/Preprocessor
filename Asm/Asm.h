#ifndef TO_BYTE_CODE_H
#define TO_BYTE_CODE_H

#include <stdio.h>

#include "Input.h"

#include "../Errors.h"
#include "../Config.h"

Errors process_input_commands_bin(FILE *dest, const Data *src, FILE *labels);

#endif // TO_BYTE_CODE_H