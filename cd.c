#include "header.h"

char olddir[1024] = "";

void cd(char **arg, int num_arg)
{
    char *curr_dir = getcwd(NULL, 0);
    char *newwd = (char *)malloc(sizeof(char) * 1024);

    if (num_arg == 2)
    {
        if (strcmp(arg[1], "~") == 0)
        {
            strcpy(newwd, home_dir);
            strcpy(olddir, curr_dir);
        }
        else if (strcmp(arg[1], "-") == 0)
        {
            if (strcmp(curr_dir, home_dir) == 0)
            {
                if (strcmp(olddir, "") == 0)
                {
                    printf("cd : OLDPWD not set\n");
                    return;
                }
            }
            strcpy(newwd, olddir);
            printf("%s\n", olddir);
            strcpy(olddir, curr_dir);
        }
        else if (strcmp(arg[1], ".") == 0)
        {
            strcpy(newwd, curr_dir);
            strcpy(olddir, curr_dir);
        }
        else if (strcmp(arg[1], "..") == 0)
        {
            if (strcmp(curr_dir, home_dir) == 0)
            {
                strcpy(newwd, "..");
                strcpy(olddir, curr_dir);
                chdir(newwd);
                printf("\033[1;31m%s\033[0m\n", getcwd(NULL, 0));
                return;
            }
            strcpy(newwd, "..");
            strcpy(olddir, curr_dir);
        }
        else if (strcmp(arg[1], "/") == 0)
        {
            strcpy(newwd, "/");
            strcpy(olddir, curr_dir);
        }
        else
        {
            if (arg[1][0] == '/')
            {
                strcpy(newwd, arg[1]);
                strcpy(olddir, curr_dir);
            }
            else
            {
                strcpy(newwd, curr_dir);
                strcat(newwd, "/");
                strcat(newwd, arg[1]);
                if (chdir(newwd) == 0)
                {
                    strcpy(olddir, curr_dir);
                    return;
                }
                strcpy(newwd, arg[1]);
                if (chdir(newwd) == 0)
                {
                    strcpy(olddir, curr_dir);
                    return;
                }
            }
        }
    }
    else if (num_arg == 1)
    {
        strcpy(newwd, home_dir);
        strcpy(olddir, curr_dir);
    }
    // else if (num_arg > 2)
    // {
    //     printf("cd : Too many arguments\n");
    //     strcpy(newwd, curr_dir);
    // }
    int ret_val = chdir(newwd);
    free(newwd);
    free(curr_dir);
    if (ret_val < 0)
    {
        perror("Erorr");
    }
}
