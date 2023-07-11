#include "header.h"

void pinfo(char **arg, int num)
{
    int pid = -1;
    char *status = (char *)malloc(sizeof(char) * 1024);
    char *path = (char *)malloc(sizeof(char) * 1024);
    char *path_exe = (char *)malloc(sizeof(char) * 1024);
    int pgrp = -1;
    int tpgid = -1;

    if (num == 1)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(arg[1]);
    }
    sprintf(path, "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("");
        return;
    }

    printf("pid : %d\n", pid);
    size_t temp = 1024;
    int size = getline(&status, &temp, file);
    char *token = (char *)malloc(sizeof(char) * 1024);
    token = strtok(status, " ");
    int val = 1;
    while (token != NULL)
    {
        if (val == 3)
        {
            printf("Process Status : %s", token);
        }
        else if (val == 5)
        {
            pgrp = atoi(token);
        }
        else if (val == 8)
        {
            tpgid = atoi(token);
            if (tpgid == pgrp)
            {
                printf("+\n");
            }
            else
            {
                printf("\n");
            }
        }
        else if (val == 23)
        {
            printf("memory : %s {Virtual Memory}\n", token);
        }
        token = strtok(NULL, " ");
        val++;
    }

    fclose(file);
    strcpy(path, "");
    sprintf(path, "/proc/%d/exe", pid);

    int read = readlink(path, path_exe, 1024);
    path_exe[read] = '\0';

    int flag = 1;
    for (int i = 0; i < homelen; i++)
    {
        if (path_exe[i] != home_dir[i])
        {
            flag = 0;
            break;
        }
    }
    if (flag)
    {
        path_exe[homelen - 1] = '~';
        printf("executable path : %s\n", &path_exe[homelen - 1]);
    }
    else
        printf("executable path : %s\n", path_exe);

    free(status);
    free(path);
    free(path_exe);
    free(token);
}