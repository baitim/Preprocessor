#include <assert.h>
#include <string.h>
#include <math.h>

#include "ANSI_colors.h"
#include "Calculate.h"
#include "../Stack/Stack.h"
#include "Commands.h"
#include "Config.h"
#include "Output.h"

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
            case 1:  push(&stack, src);         break;
            case 2:  in(&stack);                break;
            case 3:  pop(&stack, src);          break;
            case 4:  add(&stack);               break;
            case 5:  sub(&stack);               break;
            case 6:  mul(&stack);               break;
            case 7:  div(&stack);               break;
            case 8:  sqrt(&stack);              break;
            case 9:  sin(&stack);               break;
            case 10: cos(&stack);               break;
            case 11: out(&stack);               break;
            case 12: return;
            default: assert(0);
        }
        //print_commands_stack(name_of_file, number_command);

        number_command++;
    }

    stack_dtor(&stack);
}

static void push(Stack *stack, FILE *src)
{
    char *value = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
    assert(value);

    int count_input = fscanf(src, "%s", value);
    assert(count_input == 1);

    for (int i = 0; i < COUNT_REGISTERS; i++) {
        if (strcmp(REGISTERS[i].name, value) == 0) {
            stack_push(stack, REGISTERS[i].value);
            return;
        }
    }

    stack_push(stack, atoi(value));
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
    char *name_reg = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
    assert(name_reg);
    int count_input = fscanf(src, "%s", name_reg);
    assert(count_input == 1);

    int x = 0;
    stack_pop(stack, &x);

    for (int i = 0; i < COUNT_REGISTERS; i++) {
        if (strcmp(REGISTERS[i].name, name_reg) == 0) {
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
