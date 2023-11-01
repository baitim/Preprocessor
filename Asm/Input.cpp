#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../Config.h"
#include "Input.h"

static GlobalErrors get_data(DATA *data, FILE *stream);

static int count_pointers(const char *text);

static GlobalErrors write_pointers(DATA *data);

GlobalErrors create_data(DATA *data, const char *src)
{
    FILE *stream = fopen(src, "r");
    if (!stream)
        return GLOBAL_ERROR_READ_FILE;

    if (fsize(&data->size_file, src))
        return GLOBAL_ERROR_READ_FILE;
    data->size_file++;

    data->text = (char *) calloc(data->size_file, sizeof(char));
    if (!data->text)
        return GLOBAL_ERROR_ALLOC_FAIL;

    if (get_data(data, stream)) 
        return GLOBAL_ERROR_READ_FILE;

    data->commands_count = count_pointers(data->text) + 1;
    data->pointers = (char **) calloc(data->commands_count, sizeof(char *));
    if (!data->pointers)
        return GLOBAL_ERROR_ALLOC_FAIL;

    if (write_pointers(data))
        return GLOBAL_ERROR_READ_FILE;

    fclose(stream);
    return GLOBAL_ERROR_NO;
}

GlobalErrors fsize(int *size_file, const char *filename) {
    struct stat st = {};

    if (stat(filename, &st) == 0)
        *size_file = (int)st.st_size;
    else 
        return GLOBAL_ERROR_READ_FILE;

    return GLOBAL_ERROR_NO;
}

static GlobalErrors get_data(DATA *data, FILE *stream)
{
    assert(data->text);
    assert(stream);

    const int size = 1;

    if ((int)fread(data->text, data->size_file, size, stream) != 0)
        return GLOBAL_ERROR_ALLOC_FAIL;

    data->text[data->size_file - 1] = '\0';
    return GLOBAL_ERROR_NO;
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

static GlobalErrors write_pointers(DATA *data)
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
    return GLOBAL_ERROR_NO;
}

void dtor_data(DATA *data)
{
    assert (data);
    data->size_file = POISON_BYTE;
    free(data->text);
    free(data->pointers);
    data->commands_count = POISON_BYTE;
}
