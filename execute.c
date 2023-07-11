#include "header.h"

int stdin_fd;
int stdout_fd;

void reset_IO()
{
    if (dup2(stdin_fd, STDIN_FILENO) == -1 || dup2(stdout_fd, STDOUT_FILENO) == -1)
    {
        perror("");
    }

    close(stdin_fd);
    close(stdout_fd);
    return;
}

int tokenise_com(char *input, char **list)
{
    char *token = strtok(input, " \t\n");
    int count = 0;
    while (token != NULL)
    {
        list[count] = (char *)malloc(strlen(token) * sizeof(char));
        list[count] = token;
        token = strtok(NULL, " \t\n");
        count++;
    }
    return count;
}

void before_execute(char *input)
{
    int last_arg = 0;
    stdin_fd = dup(STDIN_FILENO);
    stdout_fd = dup(STDOUT_FILENO);
    if (stdin_fd == -1 || stdout_fd == -1)
    {
        printf("Not a valid file descriptor\n");
        return;
    }
    char **arg = (char **)malloc(sizeof(char *) * 1024);
    char **new_arg = (char **)malloc(sizeof(char *) * 1024);
    fore = 1;
    back = 0;
    if (input[strlen(input) - 1] == '&')
    {
        back = 1;
        fore = 0;
        input[strlen(input) - 1] = '\0';
    }
    int num_arg = tokenise_com(input, new_arg);
    int fore_new = fore, back_new = back;
    int flag = 0;

    for (int i = 0; i < num_arg; i++)
    {
        if (strcmp(new_arg[i], "|") == 0)
        {

            int pipe_fd[2];
            if (pipe(pipe_fd) == -1)
            {
                perror("");
                return;
            }
            if (!flag)
                dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
            execute(arg, last_arg);
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);

            for (int j = 0; j < last_arg; j++)
            {
                free(arg[j]);
            }
            last_arg = 0;
            int r = dup2(stdout_fd, STDOUT_FILENO);

            if (r == -1)
            {
                perror("");
                return;
            }
        }
        else if (strcmp(new_arg[i], "<") == 0)
        {
            if (i + 1 == num_arg)
            {
                reset_IO();
                printf("Error : Missing file name after '<'\n");
                return;
            }
            int fd = open(new_arg[++i], O_RDONLY);
            if (fd == -1)
            {
                reset_IO();
                printf("Bash : %s : No such file/directory\n", new_arg[i]);
                return;
            }
            dup2(fd, STDIN_FILENO);
        }
        else if (strcmp(new_arg[i], ">") == 0)
        {
            if (i + 1 == num_arg)
            {
                reset_IO();
                printf("Error : Missing file name after '>'\n");
                return;
            }
            int fd = open(new_arg[++i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            flag = 1;
            if (fd == -1)
            {
                reset_IO();
                printf("Bash : %s : No such file/directory\n", new_arg[i]);
                return;
            }
            dup2(fd, STDOUT_FILENO);
        }
        else if (strcmp(new_arg[i], ">>") == 0)
        {
            if (i + 1 == num_arg)
            {
                reset_IO();
                printf("Error : Missing file name after '>>'\n");
                return;
            }
            int fd = open(new_arg[++i], O_WRONLY | O_CREAT | O_APPEND, 0644);
            flag = 1;
            if (fd == -1)
            {
                reset_IO();
                printf("Bash : %s : No such file/directory\n", new_arg[i]);
                return;
            }
            dup2(fd, STDOUT_FILENO);
        }
        else
        {
            arg[last_arg] = (char *)malloc(sizeof(char) * 1024);
            strcpy(arg[last_arg], new_arg[i]);
            last_arg++;
            fore = fore_new, back = back_new;
        }
    }
    if (last_arg != 0)
    {
        execute(arg, last_arg);
    }

    for (int i = 0; i < last_arg; i++)
    {
        free(arg[i]);
    }
    reset_IO();
    free(arg);
    free(new_arg);
    fore = 0;
    fflush(stdout);
}

void execute(char **arg, int num_arg)
{
    if (num_arg == 0)
    {
        return;
    }
    arg[num_arg] = (char *)malloc(sizeof(char) * 1024);
    arg[num_arg] = NULL;
    strcpy(arg[0], get_alias(arg[0]));

    if (strcmp(arg[0], "alias") == 0)
    {
        if (num_arg == 1)
        {
            print_alias();
            return;
        }
        if (num_arg != 3)
        {
            printf("Wrong arguments\n");
            return;
        }
        char *old = (char *)malloc(sizeof(char) * 1024);
        char *old_new = (char *)malloc(sizeof(char) * 1024);
        for (int i = 1; i < num_arg; i++)
        {
            if (arg[i][0] == '"')
            {
                strcpy(old, &arg[i][1]);
                old[strlen(old) - 1] = '\0';
            }
            else
            {
                strcpy(old_new, arg[i]);
            }
        }
        add_alias(old, old_new);
        free(old);
        free(old_new);
        return;
    }
    else if (strcmp(arg[0], "dalias") == 0)
    {
        int r = remove_alias(arg[1]);
        return;
    }

    if (strcmp(arg[0], "pwd") == 0)
    {
        if (num_arg == 1)
            print_cwd();
        else
        {
            printf("wrong number of input\n");
            return;
        }
    }
    else if (strcmp(arg[0], "nightswatch") == 0)
    {
        //     if(num_arg!=4)
        //     {
        //         printf("nightswatch : ")
        //     }
        nightswatch(arg, num_arg);
    }
    else if (strcmp(arg[0], "echo") == 0)
    {
        echo(arg, num_arg);
    }
    else if (strcmp(arg[0], "cd") == 0)
    {
        if (num_arg > 2)
        {
            printf("cd : too many argumnets\n");
            return;
        }
        cd(arg, num_arg);
    }
    else if (strcmp(arg[0], "replay") == 0)
    {
        replay(arg, num_arg);
    }
    else if (strcmp(arg[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(arg[0], "history") == 0)
    {
        print_history(history_path);
    }
    else if (strcmp(arg[0], "ls") == 0)
    {
        ls(arg, num_arg);
    }
    else if (strcmp(arg[0], "discover") == 0)
    {
        discover(arg, num_arg);
    }
    else if (strcmp(arg[0], "clear") == 0)
    {
        printf("\e[1;1H\e[2J");
    }
    else if (strcmp(arg[0], "pinfo") == 0)
    {
        pinfo(arg, num_arg);
    }
    else if (strcmp(arg[0], "jobs") == 0)
    {
        bg_jobs(arg, num_arg);
    }
    else if (strcmp(arg[0], "sig") == 0)
    {
        sig(arg, num_arg);
    }
    else if (strcmp(arg[0], "bg") == 0)
    {
        bg(arg, num_arg);
    }
    else if (strcmp(arg[0], "fg") == 0)
    {
        fg(arg, num_arg);
    }
    else
    {
        jobs(arg, num_arg);
    }
    return;
}