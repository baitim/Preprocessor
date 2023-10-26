#ifndef FROM_BYTE_CODE_H
#define FROM_BYTE_CODE_H

#include <stdio.h>

#include "../Errors.h"
#include "../Config.h"
#include "../Asm/Input.h"

GlobalErrors process_byte_commands_bin(FILE *dest, const char *name_src);

#endif // FROM_BYTE_CODE_H