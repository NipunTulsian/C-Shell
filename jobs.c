#include "header.h"

job_det *init()
{
    job_det *temp = (job_det *)malloc(sizeof(job_det));
    temp->name = (char *)malloc(sizeof(char) * 1024);
    temp->next = NULL;
    job_count++;
    temp->job_num = last;
    last++;
    return temp;
}

void add_job(char *name, pid_t pid)
{
    job_det *node = init();
    strcpy(node->name, name);
    node->pid = pid;

    job_det *curr = head;
    job_det *prev = NULL;
    if (head == NULL)
    {
        head = node;
        return;
    }
    while (curr->next != NULL)
    {
        if (strcmp(name, curr->name) < 0)
        {
            if (curr == head)
            {
                node->next = head;
                head = node;
                return;
            }
            prev->next = node;
            node->next = curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    curr->next = node;
    return;
}

char *get_job(pid_t pid)
{
    job_det *curr = head;
    while (curr != NULL)
    {
        if (curr->pid == pid)
        {
            return curr->name;
        }
        curr = curr->next;
    }
    return NULL;
}

void remove_job(pid_t pid)
{
    job_det *curr = head;
    job_det *prev = NULL;
    while (curr != NULL)
    {
        if (curr->pid == pid)
        {
            job_count--;
            if (head == curr)
            {
                head = head->next;
                free(curr);
                return;
            }
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void jobs(char **arg, long long int num)
{

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("");
        return;
    }
    else if (pid == 0)
    {
        setpgid(0, 0);
        int ret = execvp(arg[0], arg);
        if (ret == -1)
        {
            printf("%s : command not found\n", arg[0]);
            exit(1);
        }
    }
    else
    {
        if (fore == 1)
        {
            int status;
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(0, pid);

            time_t start_time = time(NULL);
            waitpid(pid, &status, WUNTRACED);
            time_t end_time = time(NULL);

            if (WIFSTOPPED(status))
            {
                char *name = (char *)malloc(sizeof(char) * 1024);
                for (int j = 0; j < num; j++)
                {
                    strcat(name, arg[j]);
                    strcat(name, " ");
                }
                add_job(name, pid);
                printf("\n[%d] Stopped %s [%d]\n", last - 1, name, pid);
                free(name);
            }
            fflush(stdout);
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
        }
        else if (back == 1)
        {
            char *name = (char *)malloc(sizeof(char) * 1024);
            strcpy(name, "");
            for (int i = 0; i < num; i++)
            {
                strcat(name, arg[i]);
                strcat(name, " ");
            }
            add_job(name, pid);
            printf("[%d] %d\n", job_count, pid);
        }
    }
    return;
}