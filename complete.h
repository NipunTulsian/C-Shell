#ifndef __COMPLETE_
#define __COMPLETE_

typedef struct complete
{
    char *string;
    struct complete *next;
} complete;

complete* init_complete();
void add_complete(char* final);
void print_complete();

#endif