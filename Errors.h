#ifndef ERRORS_H
#define ERRORS_H

struct ProcessErrors {
    int error;
    const char *description;
};

enum StackErrors {
    STACK_ERROR_NO =                          0,
    STACK_ERROR_STACK_EMPTY =                 1 << 0,
    STACK_ERROR_ALLOC_FAIL =                  1 << 1,
    STACK_ERROR_STACK_CAPACITY =              1 << 2,
    STACK_ERROR_STACK_SIZE =                  1 << 3,
    STACK_ERROR_STACK_CAPACITY_LESS_SIZE =    1 << 4,
    STACK_ERROR_LEFT_CANARY_STRUCT =          1 << 5,
    STACK_ERROR_RIGHT_CANARY_STRUCT =         1 << 6,
    STACK_ERROR_HASH =                        1 << 7,
    STACK_ERROR_LEFT_CANARY_DATA =            1 << 8,
    STACK_ERROR_RIGHT_CANARY_DATA =           1 << 9,
    STACK_ERROR_STACK_DATA_EMPTY =            1 << 10,
};

const ProcessErrors STACK_ERRORS[] = {
    {STACK_ERROR_NO,                          "no errors"},
    {STACK_ERROR_STACK_EMPTY,                 "pointer stack is empty"},
    {STACK_ERROR_STACK_DATA_EMPTY,            "pointer stack->data is empty"},
    {STACK_ERROR_STACK_CAPACITY,              "stack->capacity < 0"},
    {STACK_ERROR_STACK_SIZE,                  "stack->size < 0"},
    {STACK_ERROR_STACK_CAPACITY_LESS_SIZE,    "stack->capacity < stack->size"},
    {STACK_ERROR_LEFT_CANARY_STRUCT,          "stack->left_canary_struct was changed"},
    {STACK_ERROR_RIGHT_CANARY_STRUCT,         "stack->right_canary_struct was changed"},
    {STACK_ERROR_HASH,                        "stack->hash was changed"},
    {STACK_ERROR_LEFT_CANARY_DATA,            "stack->left_canary_data was changed"},
    {STACK_ERROR_RIGHT_CANARY_DATA,           "stack->right_canary_data was changed"},
    {STACK_ERROR_ALLOC_FAIL,                  "realloc fail"},
};
const int COUNT_STACK_ERRORS = sizeof(STACK_ERRORS) / sizeof(ProcessErrors);

enum GlobalErrors {
    GLOBAL_ERROR_NO =                          0,
    GLOBAL_ERROR_ALLOC_FAIL =                  1 << 0,
    GLOBAL_ERROR_READ_FILE =                   1 << 1,
    GLOBAL_ERROR_WRITE_FILE =                  1 << 2,
    GLOBAL_ERROR_INPUT_FILE =                  1 << 3,
};

const ProcessErrors GLOBAL_ERRORS[] = {
    {GLOBAL_ERROR_NO,                          "no errors"},
    {GLOBAL_ERROR_ALLOC_FAIL,                  "realloc fail"},
    {GLOBAL_ERROR_READ_FILE,                   "file read error"},
    {GLOBAL_ERROR_WRITE_FILE,                  "file write error"},
    {GLOBAL_ERROR_INPUT_FILE,                  "can't read input file"},
};
const int COUNT_GLOBAL_ERRORS = sizeof(GLOBAL_ERRORS) / sizeof(ProcessErrors);

#endif // ERRORS_H