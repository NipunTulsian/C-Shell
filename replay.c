#include "header.h"

void replay(char **arg, int num_arg)
{
    if (num_arg < 6)
    {
        printf("replay : replay -command <command> -interval <time> -period <time>\n");
        return;
    }
    char *new_arg[1024];
    int command = 0, interval = -1, period = -1;
    int count = 0;

    for (int i = 1; i < num_arg; i++)
    {
        if (strcmp(arg[i], "-command") == 0)
            command = 1;
        else if (strcmp(arg[i], "-interval") == 0)
        {
            command = 0;
            if (i + 1 == num_arg)
            {
                printf("replay : replay -command <command> -interval <time> -period <time>\n");
                return;
            }
            interval = atoi(arg[++i]);
            if (interval == 0)
            {
                printf("replay : invalid interval\n");
                return;
            }
        }
        else if (strcmp(arg[i], "-period") == 0)
        {
            if (i + 1 == num_arg)
            {
                printf("replay : replay -command <command> -interval <time> -period <time>\n");
                return;
            }
            period = atoi(arg[++i]);
            if (period == 0)
            {
                printf("replay : invalid period\n");
                return;
            }
        }
        else if (command)
        {
            new_arg[count] = (char *)malloc(sizeof(char) * 1024);
            strcpy(new_arg[count++], arg[i]);
        }
    }
    if (period > 0 && interval > 0)
    {
        for (int i = 0; i < (period / interval); i++)
        {
            sleep(interval);
            execute(new_arg, count);
        }
    }
}
