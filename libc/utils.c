#include "../kernel/system.h"
#include "../include/limits.h"
#include "../kernel/utils.h"

#define STRINGIFY(x) #x
#define INTMIN_STR STRINGIFY(-2147483648)

char* itoa(int number, char* buffer)
{
    if (number == INT_MIN)
    {
        strcpy(buffer, INTMIN_STR, 12);
        return INTMIN_STR; 
    }

    int flag = 0;
    char str[128] = { 0 };
    int i = 126;

    if (number < 0)
    {
        flag = 1;
        number = -number;
    }

    if (number == 0)
    {
        str[i--] = '0';
    }

    while (number != 0)
    {
        str[i--] = (number % 10) + '0';
        number /= 10;
    }
    if (flag) 
        str[i--] = '-';
    
    strcpy(buffer, str + i + 1, 128);
    return buffer;
}