#include <assert.h>
#include <math.h>

#include "ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "Commands.h"

static void push(Stack *stack, FILE *src);
static void add(Stack *stack);
static void sub(Stack *stack);
static void mul(Stack *stack);
static void div(Stack *stack);
static void sqrt(Stack *stack);
static void sin(Stack *stack);
static void cos(Stack *stack);
static void in(Stack *stack);
static void out(Stack *stack);

void calculate(FILE *src)
{
    Stack stack = {};
    stack_ctor(&stack);
    while (true) {
        int instruction = -1;
        int count_input = fscanf(src, "%d", &instruction);
        if (count_input != 1)
            break;
        
        switch (instruction) {
            case 1:  push(&stack, src); break;
            case 2:  in(&stack);        break;
            case 3:                     break;
            case 4:  add(&stack);       break;
            case 5:  sub(&stack);       break;
            case 6:  mul(&stack);       break;
            case 7:  div(&stack);       break;
            case 8:  sqrt(&stack);      break;
            case 9:  sin(&stack);       break;
            case 10: cos(&stack);       break;
            case 11: out(&stack);       break;
            case 12: return;
            default: assert(0);
        }
    }

    stack_dtor(&stack);
}

static void push(Stack *stack, FILE *src)
{
    int x = 0;
    int count_input = fscanf(src, "%d", &x);
    assert(count_input == 1);
    stack_push(stack, x);
}

static void add(Stack *stack)
{
    int x = 0, y = 0;
    stack_pop(stack, &x);
    stack_pop(stack, &y);
    stack_push(stack, x + y);
}

static void sub(Stack *stack)
{
    int x = 0, y = 0;
    stack_pop(stack, &y);
    stack_pop(stack, &x);
    stack_push(stack, x - y);
}

static void mul(Stack *stack)
{
    int x = 0, y = 0;
    stack_pop(stack, &x);
    stack_pop(stack, &y);
    stack_push(stack, x * y);
}

static void div(Stack *stack)
{
    int x = 0, y = 0;
    stack_pop(stack, &y);
    stack_pop(stack, &x);
    stack_push(stack, x / y);
}

static void sqrt(Stack *stack)
{
    int x = 0;
    stack_pop(stack, &x);
    stack_push(stack, (type_el)sqrt(x));
}

static void sin(Stack *stack)
{
    int x = 0;
    stack_pop(stack, &x);
    stack_push(stack, (type_el)sin(x));
}

static void cos(Stack *stack)
{
    int x = 0;
    stack_pop(stack, &x);
    stack_push(stack, (type_el)cos(x));
}

static void in(Stack *stack)
{
    printf(print_lyellow("Input element: "));
    int x = 0;
    int count_input = scanf("%d", &x);
    assert(count_input == 1);
    stack_push(stack, x);
}

static void out(Stack *stack)
{
    int answer = 0;
    stack_pop(stack, &answer);
    printf(print_lgreen("Answer = "));
    printf(print_lcyan("%d\n"), answer);
}
