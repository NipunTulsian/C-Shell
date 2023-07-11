#include "header.h"

void echo(char **arg,int num)
{
    int i=1;
    while(i<num)
    {
        printf("\033[1;35m%s \033[0m",arg[i]);
        i++;
    }
    printf("\n");
}