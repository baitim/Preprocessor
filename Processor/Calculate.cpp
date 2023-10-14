#include <assert.h>
#include <string.h>
#include <math.h>

#include "../ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "../Commands.h"
#include "../Config.h"
#include "../Output.h"

static void push(Stack *stack, FILE *src);
static void in(Stack *stack);
static void pop(Stack *stack, FILE *src);
static void add(Stack *stack);
static void sub(Stack *stack);
static void mul(Stack *stack);
static void div(Stack *stack);
static void sqrt(Stack *stack);
static void sin(Stack *stack);
static void cos(Stack *stack);
static void out(Stack *stack);

void calculate(const char *name_of_file)
{
    FILE *src = fopen(name_of_file, "r");

    Stack stack = {};
    stack_ctor(&stack);

    int number_command = 1;
    while (true) {
        int command = -1;
        int count_input = fscanf(src, "%d", &command);
        if (count_input != 1)
            break;
        
        switch (command) {
            case CMD_PUSH: push(&stack, src);         break;
            case CMD_IN:   in(&stack);                break;
            case CMD_POP:  pop(&stack, src);          break;
            case CMD_ADD:  add(&stack);               break;
            case CMD_SUB:  sub(&stack);               break;
            case CMD_MUL:  mul(&stack);               break;
            case CMD_DIV:  div(&stack);               break;
            case CMD_SQRT: sqrt(&stack);              break;
            case CMD_SIN:  sin(&stack);               break;
            case CMD_COS:  cos(&stack);               break;
            case CMD_OUT:  out(&stack);               break;
            case CMD_HLT:  return;
            default: assert(0);
        }
        //print_commands(name_of_file, number_command);

        number_command++;
    }

    stack_dtor(&stack);
    fclose(src);
}

static void push(Stack *stack, FILE *src)
{
    int value = 0;
    int count_input = fscanf(src, "%d", &value);
    assert(count_input == 1);

    for (int i = 0; i < COUNT_REGISTERS; i++) {
        if (REGISTERS[i].index == value) {
            stack_push(stack, REGISTERS[i].value);
            return;
        }
    }

    // value = get_arg();

    stack_push(stack, value);
}

static void in(Stack *stack)
{
    printf(print_lyellow("Input element: "));
    int x = 0;
    int count_input = scanf("%d", &x);
    assert(count_input == 1);
    stack_push(stack, x);
}

static void pop(Stack *stack, FILE *src)
{
    int name_reg = 0;
    int count_input = fscanf(src, "%d", &name_reg);
    assert(count_input == 1);

    int x = 0;

    // value = get_arg();
    stack_pop(stack, &x);

    for (int i = 0; i < COUNT_REGISTERS; i++) {
        if (REGISTERS[i].index == name_reg) {
            REGISTERS[i].value = x;
            return;
        }
    }
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

static void out(Stack *stack)
{
    int answer = 0;
    stack_pop(stack, &answer);
    printf(print_lgreen("Answer = "));
    printf(print_lcyan("%d\n"), answer);
}
