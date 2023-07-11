#include "header.h"

struct dirent **dir;

void filepermissions(char *path, char *name)
{
    struct stat fd;
    struct tm *time;
    int r = stat(path, &fd);
    char *perm = (char *)malloc(sizeof(char) * 1024);
    char months[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    strcpy(perm, "");
    strcat(perm, S_ISREG(fd.st_mode) ? "-" : "d");
    strcat(perm, (fd.st_mode & S_IRUSR) ? "r" : "-");
    strcat(perm, (fd.st_mode & S_IWUSR) ? "w" : "-");
    strcat(perm, (fd.st_mode & S_IXUSR) ? "x" : "-");
    strcat(perm, (fd.st_mode & S_IRGRP) ? "r" : "-");
    strcat(perm, (fd.st_mode & S_IWGRP) ? "w" : "-");
    strcat(perm, (fd.st_mode & S_IXGRP) ? "x" : "-");
    strcat(perm, (fd.st_mode & S_IROTH) ? "r" : "-");
    strcat(perm, (fd.st_mode & S_IWOTH) ? "w" : "-");
    strcat(perm, (fd.st_mode & S_IXOTH) ? "x" : "-");
    time = localtime(&(fd.st_mtime));
    printf("%s\t", perm);
    printf("%hu\t", fd.st_nlink);
    printf("%s ", getpwuid(fd.st_uid)->pw_name);
    printf("%s\t", getgrgid(fd.st_gid)->gr_name);
    printf("%lld\t", fd.st_size);
    printf("%s %d %d:%d\t", months[time->tm_mon], time->tm_mday, time->tm_hour, time->tm_min);
    if (S_ISDIR(fd.st_mode))
    {
        printf("\033[1;34m");
        printf("%s\n", name);
        printf("\033[0m");
    }
    else if (S_ISREG(fd.st_mode))
    {
        if (S_IXUSR & fd.st_mode)
        {
            printf("\033[1;32m");
            printf("%s\n", name);
            printf("\033[0m");
        }
        else
        {
            printf("%s\n", name);
        }
    }
    free(perm);
}

void ls(char **arg, long long int num_arg)
{
    char *curr_dir = (char *)malloc(sizeof(char) * 1024);
    curr_dir = getcwd(NULL, 0);
    char *path = (char *)malloc(sizeof(char) * 1024);
    strcpy(path, curr_dir);
    char *file[1024];
    int num_file = 0;
    int a = 0, l = 0; // 1 = true
    for (int i = 1; i < num_arg; i++)
    {
        if (strcmp(arg[i], "-a") == 0)
        {
            a = 1;
        }
        else if (strcmp(arg[i], "-l") == 0)
        {
            l = 1;
        }
        else if (strcmp(arg[i], "-al") == 0)
        {
            a = 1;
            l = 1;
        }
        else if (strcmp(arg[i], "-la") == 0)
        {
            a = 1;
            l = 1;
        }
        else
        {
            file[num_file] = (char *)malloc(sizeof(char) * 1024);
            strcpy(file[num_file], arg[i]);
            num_file++;
        }
    }

    if (num_file == 0)
    {
        strcpy(path, curr_dir);
        print_ls(path, a, l);
    }
    else
    {
        for (int i = 0; i < num_file; i++)
        {
            if (strcmp(file[i], ".") == 0)
            {
                strcpy(path, curr_dir);
            }
            else if (strcmp(file[i], "..") == 0)
            {
                strcpy(path, "..");
            }
            else if (strcmp(file[i], "/") == 0)
            {
                strcpy(path, "/");
            }
            else
            {
                if (file[i][0] == '/')
                {
                    strcpy(path, file[i]);
                }
                else
                {
                    strcpy(path, curr_dir);
                    strcat(path, "/");
                    strcat(path, file[i]);
                    strcpy(path, file[i]);
                }
            }

            struct stat f;
            stat(path, &f);
            if (S_ISREG(f.st_mode))
            {
                if (l == 0)
                {
                    printf("%s\n", file[i]);
                    continue;
                }
                else
                {
                    filepermissions(path, file[i]);
                    continue;
                }
            }
            if (num_file > 1)
            {
                printf("\033[1;36m%s:\033[0m\n", file[i]);
            }
            print_ls(path, a, l);
        }
    }
}

void print_ls(char *path, int a, int l)
{
    int num = scandir(path, &dir, NULL, alphasort);
    if (num == -1)
    {
        perror("");
        return;
    }
    int total = 0;
    if (l == 1)
    {
        for (int i = 0; i < num; i++)
        {
            if (dir[i]->d_name[0] == '.' && a == 0)
            {
                continue;
            }
            else
            {
                char *filepath = (char *)malloc(sizeof(char) * 1024);
                strcpy(filepath, path);
                strcat(filepath, "/");
                strcat(filepath, dir[i]->d_name);
                struct stat f;
                stat(filepath, &f);
                total += (f.st_blocks) / 2;
                free(filepath);
            }
        }
    }

    if (l == 0)
    {
        for (int i = 0; i < num; i++)
        {
            if (dir[i]->d_name[0] == '.' && a == 0)
                continue;

            char *file_path = (char *)malloc(sizeof(char) * 1024);
            sprintf(file_path, "%s/%s", path, dir[i]->d_name);
            struct stat fd;
            stat(file_path, &fd);
            if (S_ISDIR(fd.st_mode))
            {
                printf("\033[0;34m");
                printf("%s\033[0m\n", dir[i]->d_name);
            }
            else if (S_ISREG(fd.st_mode))
            {
                if (S_IXUSR & fd.st_mode)
                {
                    printf("\033[0;32m");
                    printf("%s\033[0m\n", dir[i]->d_name);
                }
                else
                {
                    printf("%s\n", dir[i]->d_name);
                }
            }
            free(file_path);
        }
    }
    else if (l == 1)
    {
        printf("\033[1;36mtotal %d\033[0m\n", total);
        for (int i = 0; i < num; i++)
        {
            if (dir[i]->d_name[0] == '.' && a == 0)
                continue;

            char *filepath = (char *)malloc(sizeof(char) * 1024);
            strcpy(filepath, path);
            strcat(filepath, "/");
            strcat(filepath, dir[i]->d_name);
            filepermissions(filepath, dir[i]->d_name);
            free(filepath);
        }
    }
}