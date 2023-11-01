#ifndef INPUT_H
#define INPUT_H

#include "../Errors.h"

struct DATA {
    int size_file;
    char *text;
    int commands_count;
    char **pointers;
};

GlobalErrors fsize(int *size_file, const char *filename);

GlobalErrors create_data(DATA *data, const char *src);

void dtor_data(DATA *data);

#endif // INPUT_H