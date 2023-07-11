#include "header.h"

#define USER_NAME 1024
#define SYSTEM_NAME 1024

char hostbuffer[SYSTEM_NAME];
char username[USER_NAME];
char *cwd;

void get_host()
{
    gethostname(hostbuffer, sizeof(hostbuffer));
}

void get_user()
{
    getlogin_r(username, USER_NAME);
}

void get_cwd()
{
    cwd = getcwd(NULL, 0);
}

void prompt()
{
    get_host();
    get_user();
    get_cwd();
    if (strstr(cwd, home_dir))
    {
        cwd[homelen - 1] = '~';
        cwd = &cwd[homelen - 1];
    }
    printf("\033[1;31m<\033[0m");
    printf("\033[1;32m");
    printf("%s@%s:", username, hostbuffer);
    printf("\033[0m");

    if (job == 0 || total_fore_time < 1)
    {
        strcpy(time_taken, "");
    }
    printf("\033[1;34m%s:%s\033[0m", cwd, time_taken);
    printf("\033[1;31m> \033[0m");
}