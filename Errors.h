#ifndef ERRORS_H
#define ERRORS_H

enum Errors {
    ERROR_NO =                          0,
    ERROR_STACK_EMPTY =                 1 << 0,
    ERROR_ALLOC_FAIL =                  1 << 1,
    ERROR_STACK_CAPACITY =              1 << 2,
    ERROR_STACK_SIZE =                  1 << 3,
    ERROR_STACK_CAPACITY_LESS_SIZE =    1 << 4,
    ERROR_LEFT_CANARY_STRUCT =          1 << 5,
    ERROR_RIGHT_CANARY_STRUCT =         1 << 6,
    ERROR_HASH =                        1 << 7,
    ERROR_LEFT_CANARY_DATA =            1 << 8,
    ERROR_RIGHT_CANARY_DATA =           1 << 9,
    ERROR_STACK_DATA_EMPTY =            1 << 10,
    ERROR_READ_FILE =                   1 << 11,
    ERROR_WRITE_FILE =                  1 << 12,
    ERROR_INPUT_FILE =                  1 << 13,
};

struct ProcessErrors {
    int error;
    const char *description;
};

const ProcessErrors errors[] = {
    {ERROR_NO,                          "no errors"},
    {ERROR_STACK_EMPTY,                 "pointer stack is empty"},
    {ERROR_STACK_DATA_EMPTY,            "pointer stack->data is empty"},
    {ERROR_STACK_CAPACITY,              "stack->capacity < 0"},
    {ERROR_STACK_SIZE,                  "stack->size < 0"},
    {ERROR_STACK_CAPACITY_LESS_SIZE,    "stack->capacity < stack->size"},
    {ERROR_LEFT_CANARY_STRUCT,          "stack->left_canary_struct was changed"},
    {ERROR_RIGHT_CANARY_STRUCT,         "stack->right_canary_struct was changed"},
    {ERROR_HASH,                        "stack->hash was changed"},
    {ERROR_LEFT_CANARY_DATA,            "stack->left_canary_data was changed"},
    {ERROR_RIGHT_CANARY_DATA,           "stack->right_canary_data was changed"},
    {ERROR_ALLOC_FAIL,                  "realloc fail"},
    {ERROR_READ_FILE,                   "file read error"},
    {ERROR_WRITE_FILE,                  "file write error"},
    {ERROR_INPUT_FILE,                  "can't read input file"}
};
const int COUNT_ERRORS = sizeof(errors) / sizeof(ProcessErrors);

#endif // ERRORS_H