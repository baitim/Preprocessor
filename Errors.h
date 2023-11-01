#ifndef ERRORS_H
#define ERRORS_H

struct Errors {
    int error;
    const char *description;
};

enum GlobalErrors {
    GLOBAL_ERROR_NO =                          0,
    GLOBAL_ERROR_ALLOC_FAIL =                  1 << 0,
    GLOBAL_ERROR_READ_FILE =                   1 << 1,
    GLOBAL_ERROR_WRITE_FILE =                  1 << 2,
    GLOBAL_ERROR_INPUT_FILE =                  1 << 3,
};

const Errors GLOBAL_ERRORS[] = {
    {GLOBAL_ERROR_NO,                          "no errors"},
    {GLOBAL_ERROR_ALLOC_FAIL,                  "realloc fail"},
    {GLOBAL_ERROR_READ_FILE,                   "file read error"},
    {GLOBAL_ERROR_WRITE_FILE,                  "file write error"},
    {GLOBAL_ERROR_INPUT_FILE,                  "can't read input file"},
};
const int COUNT_GLOBAL_ERRORS = sizeof(GLOBAL_ERRORS) / sizeof(Errors);

#endif // ERRORS_H