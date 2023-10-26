#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../Config.h"
#include "Input.h"

static Errors get_data(Data *data, FILE *stream);

static int count_pointers(const char *text);

static void write_pointers(Data *data);

Errors create_data(Data *data, const char *src)
{
    FILE *stream = fopen(src, "r");
    assert(stream);
    //????

    data->size_file = (int)fsize(src) + 1;
    //????
    data->text = (char *) calloc(data->size_file, sizeof(char));
    assert(data->text);
    //????

    get_data(data, stream);
    //????

    // puts(data->text);
    // puts("---------------");

    data->commands_count = count_pointers(data->text) + 1;
    data->pointers = (char **) calloc(data->commands_count, sizeof(char *));
    assert(data->pointers);
    //????

    write_pointers(data);
    //????

    fclose(stream);
    return ERROR_NO;
}

off_t fsize(const char *filename) {
    struct stat st = {};

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

static Errors get_data(Data *data, FILE *stream)
{
    assert(data->text);
    assert(stream);

    const int size = 1;

    int is_read = (int)fread(data->text, data->size_file, size, stream);
    assert(is_read != data->size_file);
    //????

    data->text[data->size_file - 1] = '\0';
    return ERROR_NO;
}

static int count_pointers(const char *text)
{
    assert(text);

    int i = 0;
    int count = 0;

    while (text[i] != '\0') {
        if (text[i] == '\n')
            count++;
        i++;
    }

    return count;
}

static void write_pointers(Data *data)
{
    assert(data->pointers);
    assert(data->text);

    int i = 0;
    int count = 1;

    data->pointers[0] = data->text;

    while (data->text[i] != '\0') {
        if (data->text[i] == '\n') {
            data->text[i] = '\0';
            data->pointers[count] = data->text + i + 1;
            count++;
        }
        i++;
    }
}

void dtor_data(Data *data)
{
    assert (data);
    data->size_file = POISON_BYTE;
    free(data->text);
    free(data->pointers);
    data->commands_count = POISON_BYTE;
}
