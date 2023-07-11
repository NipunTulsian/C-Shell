#include "header.h"

int parser(char **parse, char *input)
{
    char *token = strtok(input, " \t\n");
    int count = 0;
    while (token != NULL)
    {
        parse[count] = (char *)malloc(sizeof(char) * 1024);
        parse[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    return count;
}

void interrupt(int sleep_time)
{
    FILE *interrupt_file = fopen("/proc/interrupts", "r");
    if (interrupt_file == NULL)
    {
        perror("nightswatch : ");
        return;
    }
    char *first = (char *)malloc(sizeof(char) * 1024);
    memset(first, '\0', 1024);
    size_t len = 0;
    getline(&first, &len, interrupt_file);
    fclose(interrupt_file);
    char *token = strtok(first, " \t\n");
    while (token != NULL)
    {
        printf("%s ", token);
        token = strtok(NULL, " \t\n");
    }
    printf("\n");
    memset(first, '\0', 1024);
    while (1)
    {
        FILE *interrupt_file = fopen("/proc/interrupts", "r");
        for (int i = 0; i < 3; i++)
        {
            getline(&first, &len, interrupt_file);
        }
        fclose(interrupt_file);
        char *parse[10];
        int count = parser(parse, first);

        for (int i = 1; i < 5; i++)
        {
            printf("%s ", parse[i]);
        }
        // for (int i = 0; i < count; i++)
        // {
        //     free(parse[i]);
        // }

        sleep(sleep_time);
    }
    free(first);
}

void dirty(int sleep_time)
{
    while (1)
    {
        FILE *meminfo_file = fopen("/proc/meminfo", "r");
        char *first = (char *)malloc(sizeof(char) * 1024);
        memset(first, '\0', 1024);
        size_t len = 0;
        for (int i = 0; i < 17; i++)
        {
            getline(&first, &len, meminfo_file);
        }
        fclose(meminfo_file);
        char *parse[10];
        int count = parser(parse, first);
        printf("%s %s\n", parse[1], parse[2]);
        // for (int i = 0; i < count; i++)
        // {
        //     free(parse[i]);
        // }
        sleep(sleep_time);
        free(first);
    }
    
}

void newborn(int sleep_time)
{
    while (1)
    {
        FILE *loadavg_file = fopen("/proc/loadavg", "r");
        char *first = (char *)malloc(sizeof(char) * 1024);
        memset(first, '\0', 1024);
        size_t len = 0;
        getline(&first, &len, loadavg_file);
        fclose(loadavg_file);
        char *parse[10];
        int count = parser(parse, first);
        printf("%s\n", parse[count - 1]);
        // for (int i = 0; i < count; i++)
        // {
        //     free(parse[i]);
        // }
        free(first);
        sleep(sleep_time);
    }
}

void nightswatch(char **arg, int num_arg)
{
    if (num_arg != 4)
    {
        printf(" nightswatch : nightswatch -n <time> <command> \n");
        return;
    }
    int time = 0, interrupt_ = 0, newborn_ = 0, dirty_ = 0;
    for (int i = 1; i < num_arg; i++)
    {
        if (strcmp(arg[i], "-n") == 0)
        {
            if (i + 1 == num_arg)
            {
                printf(" nightswatch : nightswatch -n <time> <command> \n");
                return;
            }
            time = atoi(arg[++i]);
            if (time == 0)
            {
                printf("nightswatch : invalid time interval\n");
                return;
            }
        }
        else if (strcmp(arg[i], "interrupt") == 0)
        {
            interrupt_ = 1;
        }
        else if (strcmp(arg[i], "dirty") == 0)
        {
            dirty_ = 1;
        }
        else if (strcmp(arg[i], "newborn") == 0)
        {
            newborn_ = 1;
        }
    }
    if (newborn_ + dirty_ + interrupt_ != 1)
    {
        printf("nightswatch : error\n");
        return;
    }

    int pid = fork();
    if (pid == -1)
    {
        perror("");
        return;
    }
    else if (pid == 0)
    {
        if (interrupt_)
        {
            interrupt(time);
        }
        else if (newborn_)
        {
            newborn(time);
        }
        else if (dirty_)
        {
            dirty(time);
        }
    }
    else
    {
        char c;
        setbuf(stdout, NULL);
        enableRawMode();
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (c == 'q')
            {
                kill(pid, SIGTERM);
                break;
            }
        }
        disableRawMode();
    }
}