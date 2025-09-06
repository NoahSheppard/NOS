#include "system.h"

char command[256];
int command_index = 0;

void printf(char* string) 
{
    term_print(string, 0x0F);
}

void printc(char c) 
{
    term_putc(c, 0x0F);
}

void clear()
{
    term_init();
    printf("[NOS]/$ ");
}

void basicterm_return_handler()
{
    printc('\n');
    command[command_index++] = '\0';

    if (strcmp(command, "clear") == 0)
    {
        clear();
        command_index = 0;
        return;
    }

    printf("You put: ");
    printf(command);    
    printc('\n');
    printf("[NOS]/$ ");

    command_index = 0;
}

void basicterm_key_handler(char key)
{
    if (command_index < 255)
    {
        command[command_index] = key;
        command_index++;
        printc(key);
    }
}

void basicterm_main()
{
    clear();
}