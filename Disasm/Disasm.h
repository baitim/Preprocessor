#ifndef FROM_BYTE_CODE_H
#define FROM_BYTE_CODE_H

#include <stdio.h>

#include "../Main/Config.h"

#include "../Main/Commands.h"

Errors process_byte_commands(FILE *dest, FILE *src);

#endif // FROM_BYTE_CODE_H