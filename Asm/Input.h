#ifndef INPUT_H
#define INPUT_H

#include "../Errors.h"

struct Data {
    int size_file;
    char *text;
    int commands_count;
    char **pointers;
};

GlobalErrors fsize(int *size_file, const char *filename);

GlobalErrors create_data(Data *data, const char *src);

void dtor_data(Data *data);

#endif // INPUT_H