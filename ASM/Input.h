#ifndef INPUT_H
#define INPUT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

struct DATA {
    int size_file;
    char *text;
    int lines_count;
    char **pointers;
};

void create_data(DATA *data);

off_t fsize(const char *filename);

void get_data(DATA data, FILE *stream);

int count_pointers(const char *text);

void write_pointers(DATA data);

#endif // INPUT_H