#ifndef SPU_H
#define SPU_H

#include "../Stack/Stack.h"
#include "Config.h"

union SPU {
    struct Register;
    struct Stack;
    struct Data;
};

#endif // SPU_H