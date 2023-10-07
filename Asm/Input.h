#ifndef INPUT_H
#define INPUT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

struct Data {
    int size_file;
    char *text;
    int commands_count;
    char **pointers;
};

void create_data(Data *data, const char *src);

#endif // INPUT_H