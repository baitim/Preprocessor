#include "To_byte_code.h"

int main()
{
    FILE *src =  fopen("../txt/Input_commands.txt", "r");

    if (!src) { 
        printf("CAN'T READ FILE\n"); 
        return 0;
    }

    FILE *dest = fopen("../txt/Commands_in_byte_code.txt", "w");
    process_input_commands(dest, src);

    return 0;
}
