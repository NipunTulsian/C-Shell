#include "header.h"

void history_num()
{
    FILE *oldfile;
    oldfile = fopen(history_path, "a+");

    char *commands[21];
    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int num_com = 0;
    while (fgets(commands[num_com], 256, oldfile))
    {
        num_com++;
    }
    history_count = num_com - 1;
    return;
}

void store_history(char *input, char *history_path)
{
    FILE *oldfile;
    oldfile = fopen(history_path, "a+");

    char *commands[21];
    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int num_com = 0;
    while (fgets(commands[num_com], 256, oldfile))
    {
        num_com++;
    }

    fclose(oldfile);
    if (num_com > 0 && strcmp(input, commands[num_com - 1]) == 0)
        return;

    oldfile = fopen(history_path, "w");

    if (num_com > 0 && num_com < 20)
    {
        fprintf(oldfile, "%s", commands[0]);
    }
    for (int i = 1; i < num_com; i++)
    {
        fprintf(oldfile, "%s", commands[i]);
    }
    fprintf(oldfile, "%s", input);
    fclose(oldfile);
    for (int i = 0; i < 21; i++)
    {
        free(commands[i]);
    }
}

void print_history(char *path)
{
    FILE *file = fopen(path, "r");
    char *commands[21];
    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int num_com = 0;
    while (fgets(commands[num_com], 1024, file))
    {
        num_com++;
    }
    int limit = num_com < 10 ? num_com : 10;
    for (int i = num_com - limit; i < num_com; i++)
    {
        printf("%s", commands[i]);
    }
    for (int i = 0; i < 21; i++)
    {
        free(commands[i]);
    }
}

void uparrow(char *up_temp, int up)
{
    FILE *oldfile;
    oldfile = fopen(history_path, "a+");

    char *commands[21];
    for (int i = 0; i < 21; i++)
    {
        commands[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int num_com = 0;
    while (fgets(commands[num_com], 256, oldfile))
    {
        num_com++;
    }

    fclose(oldfile);

    strcpy(up_temp, commands[num_com - up]);
    for (int i = 0; i < 21; i++)
    {
        free(commands[i]);
    }
}