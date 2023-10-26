#include <stdlib.h>
#include <string.h>

#include "Disasm.h"
#include "../Labels.h"

static int powf(int x, int p);

#define DEF_CMD(name_cmd, num, type_args, args, code)                           \
    if (int_instruct % powf(2, FREE_BYTES)  == CMD_ ## name_cmd) {              \
        fprintf(dest, "%s", #name_cmd);                                         \
        if (args == 0) {                                                        \
            fprintf(dest, "\n");                                                \
            continue;                                                           \
        }                                                                       \
        for (int i = 0; i < args; i++) {                                        \
            int value  = *((int *)command + number_command);                    \
            number_command++;                                                   \
            fprintf(dest, " %d", value);                                        \
        }                                                                       \
        fprintf(dest, "\n");                                                    \
        continue;                                                               \
    }                                                                           \
    else
GlobalErrors process_byte_commands_bin(FILE *dest, const char *name_src)
{
    FILE *src =  fopen(name_src, "rb");
    if (!src) return GLOBAL_ERROR_READ_FILE;
    if (!dest) return GLOBAL_ERROR_READ_FILE;

    int size_file = 0;
    if (fsize(&size_file, name_src))
        return GLOBAL_ERROR_READ_FILE;

    char *command = (char *)calloc((size_file + COUNT_BYTES_IN_BINARY_TO_DECRIPTION), sizeof(char));
    if (!command)
        return GLOBAL_ERROR_ALLOC_FAIL;

    int count_read = (int)fread(command, sizeof(command[0]), size_file, src);
    if (count_read != size_file)
        return GLOBAL_ERROR_READ_FILE;
                                                        
    int number_command = COUNT_INTS_IN_BINARY_TO_DECRIPTION;
    while ((number_command - COUNT_INTS_IN_BINARY_TO_DECRIPTION) * (int)sizeof(int) < size_file) {
        int int_instruct = *((int *)command + number_command);
        number_command++;
        
        #include "../DSL"
        {;}
    }

    free(command);
    return GLOBAL_ERROR_NO;
}
#undef DEF_CMD

static int powf(int x, int p)
{
    if (p == 0) return 1;
    if (p % 2 == 1) return x * powf(x, p - 1);
    int z = powf(x, p/2);
    return z * z;
}