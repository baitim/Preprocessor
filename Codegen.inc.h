DEF_CMD(PUSH, 1, REG | NUM | MEM, 1, {
    int value_elem = commands[number_command];
    number_command++;

    int push_value = 0;

    if (IS_REG) {
        for (int i = 0; i < COUNT_REGISTERS; i++) {
            if (REGISTERS[i].index == value_elem) {
                push_value = REGISTERS[i].value;
                break;
            }
        }
    }

    if (IS_MEM) {
        if (IS_NUM)
            PUSH(ram[value_elem]);
        else
            PUSH(ram[push_value]);
    } else {
        if (IS_NUM)
            PUSH(value_elem * PRECISION);
        else
            PUSH(push_value);
    }

    break;
})

DEF_CMD(IN, 2, NUN, 0, {
    printf(print_lyellow("Input element: "));
    int x = 0;
    int count_input_ = scanf("%d", &x);
    assert(count_input_ == 1);
    PUSH(x * PRECISION);
    break;
})

DEF_CMD(POP, 3, NUM | REG | MEM, 1, {
    int place_to_write = commands[number_command];
    number_command++;

    int x = 0;
    POP(&x);

    int number_reg = 0;

    if (IS_REG) {
        for (int i = 0; i < COUNT_REGISTERS; i++) {
            if (REGISTERS[i].index == place_to_write) {
                number_reg = i;
                break;
            }
        }
    }

    if (IS_MEM) {
        if (IS_NUM)
            ram[place_to_write] = x;
        else
            ram[REGISTERS[number_reg].value / PRECISION] = x;
    } else {
        if (IS_REG)
            REGISTERS[number_reg].value = x;
    }

    break;
})

DEF_CMD(ADD, 4, NUN, 0, {
#ifdef PRINT_COMMANDS
    printf("ADD\n");
    print_stack(&stack);
#endif
    int x = 0;
    int y = 0;
    PUSH((POP(&x), x) + (POP(&y), y));
#ifdef PRINT_COMMANDS
    printf("ADDED VALUE = %d\n", x + y);
#endif
    break;
})

DEF_CMD(SUB, 5, NUN, 0, {
#ifdef PRINT_COMMANDS
    printf("SUB\n");
    print_stack(&stack);
#endif
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    PUSH(y - x);
    break;
})

DEF_CMD(MUL, 6, NUN, 0, {
#ifdef PRINT_COMMANDS
    printf("MUL\n");
    print_stack(&stack);
#endif
    int x = 0;
    int y = 0;
    PUSH((POP(&x), x) * (POP(&y), y) / PRECISION);
    break;
})

DEF_CMD(DIV, 7, NUN, 0, {
#ifdef PRINT_COMMANDS
    printf("DIV\n");
    print_stack(&stack);
#endif
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    PUSH(y * PRECISION / x);
    break;
})

DEF_CMD(SQRT, 8, NUN, 0, {
    int x = 0;
    POP(&x);
    PUSH((int)sqrt((double)x * (double)100));
    break;
})

DEF_CMD(SIN, 9, NUN, 0, {
    int x = 0;
    POP(&x);
    PUSH((int)sin((double)x / (double)100));
    break;
})

DEF_CMD(COS, 10, NUN, 0, {
    int x = 0;
    POP(&x);
    PUSH((int)cos((double)x / (double)100));
    break;
})

DEF_CMD(OUT, 11, NUN, 0, {
    int answer = 0;
    printf(print_lcyan("%.3lf"), double((POP(&answer), answer)) / PRECISION);
    break;
})

DEF_CMD(HLT, 12, NUN, 0, {
    return GLOBAL_ERROR_NO;
    break;
})

DEF_CMD(JMP, 13, NUM, 1, {
    number_command = commands[number_command];
    break;
})

DEF_CMD(JA, 14, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y > x) number_command = commands[number_command];
    else       number_command++;
    break;
})

DEF_CMD(JAE, 15, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y >= x) number_command = commands[number_command];
    else        number_command++;
    break;
})

DEF_CMD(JB, 16, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y < x) number_command = commands[number_command];
    else       number_command++;
    break;
})

DEF_CMD(JBE, 17, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y <= x) number_command = commands[number_command];
    else        number_command++;
    break;
})

DEF_CMD(JE, 18, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y == x) number_command = commands[number_command];
    else        number_command++;
    break;
})

DEF_CMD(JNE, 19, NUM, 1, {
    int x = 0;
    int y = 0;
    POP(&x);
    POP(&y);
    if (y != x) number_command = commands[number_command];
    else        number_command++;
    break;
})

DEF_CMD(JMON, 20, NUM, 1, {
    long int time_ = time(NULL);
    struct tm *loc_time;
    loc_time = localtime(&time_);
    if (loc_time->tm_wday == 1) number_command = commands[number_command];
    else                        number_command++;
    break;
})

DEF_CMD(CALL, 21, NUM, 1, {
    PUSH_COM(number_command + 1);
    number_command = commands[number_command];
    break;
})

DEF_CMD(RET, 22, NUN, 0, {
    POP_COM(&number_command);
    break;
})

DEF_CMD(OUTC, 23, NUN, 0, {
    int print_value = 0;
    printf(print_lyellow("%c"), ((POP(&print_value), print_value) / PRECISION));
    break;
})

DEF_CMD(DRAW, 24, NUN, 0, {
    for (int index_ = START_DRAW_RAM; index_ <= END_DRAW_RAM; index_++) {
        if (ram[index_] < 0) continue;
        printf(print_lcyan("%c"), ram[index_] / PRECISION);
    }
    break;
})

DEF_CMD(CLEAR_DRAW, 25, NUN, 0, {
    for (int index_ = START_DRAW_RAM; index_ <= END_DRAW_RAM; index_++) {
        ram[index_] = POISON_DRAW;
    }
    break;
})