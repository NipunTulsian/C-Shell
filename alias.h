#ifndef __ALIAS_
#define __ALIAS_

typedef struct alias
{
    char *old;
    char *old_new;
    struct alias *next;
} alias;

alias *init_alias();
char *get_alias(char *old);
void add_alias(char *old, char *old_new);
int remove_alias(char *old_new);
void print_alias();

#endif