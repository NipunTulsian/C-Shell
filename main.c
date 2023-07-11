#include "header.h"

void handler()
{
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        char *name = (char *)malloc(sizeof(char) * 1024);
        char *temp_com = (char *)malloc(sizeof(char) * 1024);
        name = get_job(pid);
        if (name == NULL)
        {
            continue;
        }

        if (WIFEXITED(status))
        {
            // printf("%s with pid %d exited normally\n", name, pid);
            sprintf(temp_com, "%s with pid %d exited normally", name, pid);
            add_complete(temp_com);
            remove_job(pid);
            if (job_count == 0)
            {
                last = 1;
            }
        }
        else if (WIFSTOPPED(status))
        {
            printf("++=\n");
            return;
        }
        else
        {
            sprintf(temp_com, "%s with pid %d exited abnormally", name, pid);
            add_complete(temp_com);
            remove_job(pid);
            if (job_count == 0)
            {
                last = 1;
            }
        }
        // if (fore != 1)
        //     prompt();
        fflush(stdout);
        free(name);
        free(temp_com);
    }
}

void tokenise(char *input)
{
    char *token = strtok(input, ";\n");
    int count = 0;
    char **arg = (char **)malloc(sizeof(char *) * 1024);
    while (token != NULL)
    {
        arg[count] = (char *)malloc(sizeof(char) * 1024);
        strcpy(arg[count], token);
        token = strtok(NULL, ";\n");
        count++;
    }
    if (count == 0)
    {
        return;
    }
    free(token);
    int sum = 0;
    char **arg_and = (char **)malloc(sizeof(char *) * 1024);
    for (int i = 0; i < count; i++)
    {
        int num = 0;
        for (int j = 0; j < strlen(arg[i]); j++)
        {
            if (arg[i][j] == '&')
                num++;
        }
        char *token_and = strtok(arg[i], "&\n");
        int tmp = 0;
        while (token_and != NULL)
        {
            arg_and[sum] = (char *)malloc(sizeof(char) * 1024);
            strcpy(arg_and[sum], token_and);
            token_and = strtok(NULL, "&\n");
            tmp++;
            sum++;
        }

        for (int j = sum - tmp; num > 0; j++)
        {
            strcat(arg_and[j], "&");
            num--;
        }
    }
    for (int i = 0; i < sum; i++)
    {
        before_execute(arg_and[i]);
    }
}

void chandler()
{
    if (fore == 0)
    {
        char host[1024], username[1024];
        char *cwd;
        gethostname(host, sizeof(host));
        getlogin_r(username, 1024);
        cwd = getcwd(NULL, 0);
        if (strstr(cwd, home_dir))
        {
            cwd[homelen - 1] = '~';
            cwd = &cwd[homelen - 1];
        }
        printf("^C");
        printf("\033[1;31m\n<\033[0m");
        printf("\033[1;32m");
        printf("%s@%s:", username, host);
        printf("\033[0m");

        if (job == 0 || total_fore_time < 1)
        {
            strcpy(time_taken, "");
        }
        printf("\033[1;34m%s:%s\033[0m", cwd, time_taken);
        printf("\033[1;31m> \033[0m");

        fflush(stdout);
        fflush(stdin);
    }
}

void zhandler()
{
    // printf("\b\b  \b\b");
    fflush(stdout);
    fflush(stdin);
}

int main()
{
    char *input = (char *)malloc(sizeof(char) * 1024);
    char *input_copy = (char *)malloc(sizeof(char) * 1024);

    char c;
    fore = 0, back = 0;
    strcpy(time_taken, "");

    size_t bufsize = 256;
    home_dir = getcwd(NULL, 0);
    homelen = strlen(home_dir);

    strcpy(history_path, home_dir);
    strcat(history_path, "/.history");
    history_num();
    job_count = 0, last = 1;
    head = NULL;
    head_alias = NULL;
    head_complete = NULL;
    signal(SIGCHLD, handler);
    signal(SIGINT, chandler);
    signal(SIGTSTP, zhandler);

    while (1)
    {
        if (head_complete != NULL)
        {
            print_complete();
        }

        prompt();
        job = 0;

        total_fore_time = 0;
        strcpy(time_taken, "");

        setbuf(stdout, NULL);
        enableRawMode();
        memset(input, '\0', 1024);
        memset(input_copy, '\0', 1024);
        int pt = 0, left = 0, up = 0;
        char *left_arr = (char *)malloc(sizeof(char) * 1024);
        memset(left_arr, '\0', 1024);
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        input[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == '\033')
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    {
                        if (buf[1] == 'D')
                        {
                            if (pt == 0 || left == pt)
                                continue;
                            left_arr[left] = input[pt - left - 1];
                            left = strlen(left_arr);
                            printf("\b");
                        }
                        else if (buf[1] == 'C')
                        {
                            if (left == 0)
                                continue;
                            printf("%c", left_arr[left - 1]);
                            left_arr[--left] = '\0';
                        }
                        else
                        {
                            history_count = 0;
                            history_num();
                            if (buf[1] == 'A')
                            {
                                if (up == 0)
                                {
                                    strcpy(input_copy, input);
                                }
                                if (history_count == up || history_count == 0)
                                {
                                    printf("\a");
                                    continue;
                                }
                                up++;
                            }
                            else if (buf[1] == 'B')
                            {
                                if (up == 0 || history_count == 0)
                                {
                                    printf("\a");
                                    continue;
                                }
                                else if (up == 1)
                                {
                                    while (pt)
                                    {
                                        input[pt--] = '\0';
                                        printf("\b \b");
                                    }
                                    for (int i = 0; i < strlen(input_copy); i++)
                                    {
                                        printf("%c", input_copy[i]);
                                        input[pt++] = input_copy[i];
                                    }
                                    up--;
                                    continue;
                                }
                                up--;
                            }
                            char *up_temp = (char *)malloc(sizeof(char) * 1024);
                            memset(up_temp, '\0', 1024);
                            uparrow(up_temp, up);
                            while (pt)
                            {
                                input[pt--] = '\0';
                                printf("\b \b");
                            }
                            for (int i = 0; i < strlen(up_temp) - 1; i++)
                            {
                                input[pt++] = up_temp[i];
                                printf("%c", up_temp[i]);
                            }
                            free(up_temp);
                        }
                    }
                }
                else if (c == 9)
                {
                    if (pt == 0)
                    {
                        printf("\n");
                        struct dirent **dir;
                        int n = scandir(".", &dir, NULL, alphasort);
                        for (int i = 0; i < n; i++)
                        {
                            if (dir[i]->d_type == DT_DIR)
                            {
                                strcat(dir[i]->d_name, "/");
                                printf("%s\n", dir[i]->d_name);
                            }
                            else
                            {
                                printf("%s\n", dir[i]->d_name);
                            }
                        }

                        prompt();
                    }
                    else if (pt > 0)
                    {
                        // if (input[pt - 1] != ' ')
                        // {
                        char *temp = (char *)malloc(sizeof(char) * 1024);
                        char *temp_new = (char *)malloc(sizeof(char) * 1024);
                        char **match_name = (char **)malloc(sizeof(char *) * 1024);
                        memset(temp, '\0', 1024);
                        memset(temp_new, '\0', 1024);
                        int count = 0;
                        for (int i = pt - 1; i >= 0 && input[i] != ' '; i--)
                        {
                            temp[count++] = input[i];
                        }
                        for (int i = count - 1; i >= 0; i--)
                        {
                            temp_new[count - 1 - i] = temp[i];
                        }
                        memset(temp, '\0', 1024);
                        int matched = 0;
                        temp = auto_complete(temp_new, &matched, match_name);
                        if (matched > 1)
                        {
                            prompt();
                            for (int i = 0; i < pt; i++)
                            {
                                printf("%c", input[i]);
                            }
                        }
                        int count2 = 0;
                        if (temp != NULL)
                            count2 = strlen(temp);

                        if (matched == 1)
                        {
                            for (int i = 0; i < count2; i++)
                            {
                                temp_new[count++] = temp[i];
                            }
                            char path[1024];
                            strcpy(path, "./");
                            strcat(path, temp_new);
                            struct stat fd;
                            int r = stat(temp_new, &fd);
                            if (S_ISDIR(fd.st_mode))
                            {
                                temp[count2++] = '/';
                            }
                            else if (S_ISREG(fd.st_mode))
                            {
                                temp[count2++] = ' ';
                            }
                        }
                        for (int i = 0; i < count2; i++)
                        {
                            input[pt++] = temp[i];
                            printf("%c", temp[i]);
                        }

                        free(temp), free(temp_new);
                        //}
                    }
                }
                else if (c == 4)
                {
                    printf("\n");
                    exit(0);
                }
            }
            else
            {
                if (left != 0)
                {
                    for (int i = 0; i < strlen(left_arr); i++)
                    {
                        input[--pt] = '\0';
                    }
                    input[pt++] = c;
                    printf("%c", c);
                    for (int i = 0; i < strlen(left_arr); i++)
                    {
                        printf("%c", left_arr[strlen(left_arr) - 1 - i]);
                        input[pt++] = left_arr[strlen(left_arr) - 1 - i];
                    }
                    for (int i = 0; i < left; i++)
                    {
                        printf("\b");
                    }
                }
                else
                {
                    input[pt++] = c;
                    printf("%c", c);
                }
            }
        }
        disableRawMode();
        printf("\n");
        input[pt++] = '\n';

        if (strcmp(input, "\n") != 0)
            store_history(input, history_path);
        // if(strstr(input,";;")!=NULL)
        // {
        //     printf("Bash : Syntax error unexpected token ';;'\n");
        //     continue;
        // }
        tokenise(input);
        sprintf(time_taken, "took %d", total_fore_time);
    }
    return 0;
}