#ifndef __JOB_
#define __JOB_

typedef struct job_det
{
    pid_t pid;
    char *name;
    int job_num;
    struct job_det *next;
} job_det;


job_det *init();
char *get_job(pid_t pid);
void add_job(char *name, pid_t pid);
void remove_job(pid_t pid);

void jobs(char **arg, long long int num);

#endif