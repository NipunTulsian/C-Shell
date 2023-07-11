#include "header.h"

char state(char *path)
{
    char temp;
    FILE *file = fopen(path, "r");
    fscanf(file, "%*d %*s %c", &temp);
    fclose(file);
    return temp;
}

void bg_jobs(char **arg, int num_arg)
{
    int r = 0, s = 0;
    if (head == NULL)
    {
        printf("No background process\n");
        return;
    }
    for (int i = 0; i < num_arg; i++)
    {
        if (strcmp(arg[i], "-r") == 0)
        {
            r = 1;
        }
        if (strcmp(arg[i], "-s") == 0)
        {
            s = 1;
        }
    }
    if (r + s == 0)
    {
        r = s = 1;
    }
    job_det *curr = head;
    while (curr != NULL)
    {
        char *path = (char *)malloc(sizeof(char) * 1024);
        sprintf(path, "/proc/%d/stat", curr->pid);
        char curr_state = state(path);
        if (r == 1 && curr_state != 'T')
        {
            printf("[%d] Running %s [%d]\n", curr->job_num, curr->name, curr->pid);
        }
        else if (s == 1)
        {
            printf("[%d] Stopped %s [%d]\n", curr->job_num, curr->name, curr->pid);
        }
        free(path);
        curr = curr->next;
    }
    return;
}

void sig(char **arg, int num)
{
    if (num != 3)
    {
        printf("sig: Inavlid number of commands\n");
        return;
    }
    job_det *curr = head;
    int id = atoi(arg[1]);
    int signal = atoi(arg[2]);
    while (curr)
    {
        if (curr->job_num == id)
        {
            int r = kill(curr->pid, signal);
            if (r == -1)
            {
                perror("");
            }
            return;
        }
        curr = curr->next;
    }
    printf("sig : No process with job number is running\n");
}

void bg(char **arg, int num)
{
    if (num == 1)
    {
        printf("bg : job number not provided\n");
        return;
    }
    for (int i = 1; i < num; i++)
    {
        job_det *curr = head;
        int id = atoi(arg[i]);
        int flag = 0;
        while (curr != NULL)
        {
            if (curr->job_num == id)
            {
                if (kill(curr->pid, 18) == -1)
                {
                    perror("");
                }
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("bg : No background job corresponding to the given job number exists\n");
        }
    }
    return;
}

void fg(char **arg, int num)
{
    if (num == 1)
    {
        printf("bg : job number not provided\n");
        return;
    }

    job_det *curr = head;
    int id = atoi(arg[1]);
    while (curr != NULL)
    {
        if (curr->job_num == id)
        {
            time_t start_time = time(NULL);
            if (kill(curr->pid, 18) == -1)
            {
                perror("");
            }
            printf("%s\n", curr->name);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            fore = 1;
            tcsetpgrp(0, curr->pid);
            int status;
            waitpid(curr->pid, &status, WUNTRACED);
            time_t end_time = time(NULL);

            if (!WIFSTOPPED(status))
            {
                remove_job(curr->pid);
                if (job_count == 0)
                    last = 1;
            }
            else if (WIFSTOPPED(status))
            {
                printf("\n[%d] Stopped %s [%d]\n", last - 1, curr->name, curr->pid);
            }

            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            time_t time_total = end_time - start_time;
            job = 1;
            fore = 0;
            total_fore_time += time_total;
            if (head_complete != NULL)
            {
                print_complete();
            }
            return;
        }
        curr = curr->next;
    }
    printf("fg : No job with the given job number exists\n");
    return;
}