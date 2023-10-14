#ifndef FROM_BYTE_CODE_H
#define FROM_BYTE_CODE_H

#include <stdio.h>

#include "../Errors.h"
#include "../Config.h"
#include "../Commands.h"

Errors process_byte_commands_txt(FILE *dest, FILE *src);

Errors process_byte_commands_bin(FILE *dest, const char *str_src);

#endif // FROM_BYTE_CODE_H