#include "header.h"
struct dirent *dir;

void check_file(char *path, char *name, int *check, int d, int f)
{
    DIR *file;

    file = opendir(path);
    if (file == NULL)
    {
        perror("");
        return;
    }

    while ((dir = readdir(file)) != NULL)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            char *temp = (char *)malloc(sizeof(char) * 1024);
            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, dir->d_name);
            if (strcmp(dir->d_name, name) == 0)
            {
                if (dir->d_type == DT_DIR && d == 1)
                {
                    printf("\033[0;34m");
                    printf("%s\n", temp);
                    printf("\033[0m");
                    *check = 1;
                }
                if (dir->d_type == DT_REG && f == 1)
                {
                    printf("%s\n", temp);
                    *check = 1;
                }
            }
            if (dir->d_type == DT_DIR)
            {
                check_file(temp, name, check, d, f);
            }
            free(temp);
        }
    }
    closedir(file);
}

void write_content(char *path, int d, int f)
{

    DIR *file;

    file = opendir(path);
    if (file == NULL)
    {
        perror("");
        return;
    }

    if (d == 1 || d + f == 0)
    {
        printf("\033[0;34m");
        printf("%s\n", path);
        printf("\033[0m");
    }
    while ((dir = readdir(file)) != NULL)
    {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
        {
            char *temp = (char *)malloc(sizeof(char) * 1024);
            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, dir->d_name);
            if (dir->d_type == DT_DIR)
            {
                write_content(temp, d, f);
            }
            else
            {
                if (f == 1 || d + f == 0)
                    printf("%s\n", temp);
            }
            free(temp);
        }
    }
    closedir(file);
}

void discover(char **arg, long long int num)
{
    char *path = (char *)malloc(sizeof(char) * 1024);
    strcpy(path, ".");
    char *target_dir = (char *)malloc(sizeof(char) * 1024);
    strcpy(target_dir, "");
    int d = 0, f = 0;
    char *file_name = (char *)malloc(sizeof(char) * 1024);
    strcpy(file_name, "");
    for (int i = 1; i < num; i++)
    {
        if (strcmp(arg[i], "-d") == 0)
        {
            d = 1;
        }
        else if (strcmp(arg[i], "-f") == 0)
        {
            f = 1;
        }
        else if (arg[i][0] == '"')
        {
            strcpy(file_name, &arg[i][1]);
            file_name[strlen(file_name) - 1] = '\0';
        }
        else
        {
            strcpy(target_dir, arg[i]);
        }
    }
    if (d + f == 0)
    {
        d = f = 1;
    }
    if (strcmp(file_name, "") == 0)
    {
        if (num == 1)
        {
            write_content(path, d, f);
        }
        else if (strcmp(target_dir, "") == 0)
        {
            write_content(path, d, f);
        }
        else
        {
            if (target_dir[0] == '.')
            {
                strcpy(path, target_dir);
            }
            else if (strcmp(target_dir, "..") == 0)
            {
                strcpy(path, "..");
            }
            else if (strcmp(target_dir, "~") == 0)
            {
                strcpy(path, home_dir);
            }
            else if (target_dir[0] == '/')
            {
                strcpy(path, target_dir);
            }
            else
            {
                strcpy(path, ".");
                strcat(path, "/");
                strcat(path, target_dir);
            }
            write_content(path, d, f);
        }
    }
    else
    {
        int check = 0;
        if (strcmp(target_dir, "") == 0)
        {
            check_file(path, file_name, &check, d, f);
        }
        else
        {
            if (target_dir[0] == '.')
            {
                strcpy(path, target_dir);
            }
            else if (strcmp(target_dir, "..") == 0)
            {
                strcpy(path, "..");
            }
            else if (strcmp(target_dir, "~") == 0)
            {
                strcpy(path, home_dir);
            }
            else if (target_dir[0] == '/')
            {
                strcpy(path, target_dir);
            }
            else
            {
                strcpy(path, ".");
                strcat(path, "/");
                strcat(path, target_dir);
            }
            check_file(path, file_name, &check, d, f);
        }
        if (check == 0)
        {
            printf("No such file/directory\n");
        }
    }
    free(path);
    free(target_dir);
    free(file_name);
    return;
}