#include "header.h"

void print_cwd()
{
    char *cwd;
    cwd = getcwd(NULL, 0);
    printf("\033[1;31m");
    printf("%s", cwd);
    printf("\033[0m");
    printf("\n");
    free(cwd);
    return;
}