#ifndef INPUT_H
#define INPUT_H

struct Data {
    int size_file;
    char *text;
    int commands_count;
    char **pointers;
};

off_t fsize(const char *filename);

void create_data(Data *data, const char *src);

void dtor_data(Data *data);

#endif // INPUT_H