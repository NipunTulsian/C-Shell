#ifndef _BG_JOB__
#define _BG_JOB__

void bg_jobs(char **arg, int num_arg);
char state(char *path);
void sig(char **arg, int num_arg);
void bg(char **arg, int num_arg);
void fg(char **arg, int num);

#endif