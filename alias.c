#include "header.h"

alias *init_alias()
{
    alias *temp = (alias *)malloc(sizeof(alias));
    temp->next = NULL;
    temp->old = (char *)malloc(sizeof(char) * 1024);
    temp->old_new = (char *)malloc(sizeof(char) * 1024);
    return temp;
}

void add_alias(char *old, char *old_new)
{
    alias *node = init_alias();
    strcpy(node->old, old);
    strcpy(node->old_new, old_new);

    alias *curr = head_alias;
    alias* prev=NULL;
    if (head_alias == NULL)
    {
        head_alias = node;
        return;
    }
    while (curr != NULL)
    {
        if (strcmp(curr->old_new, old_new) == 0 || strcmp(curr->old_new, old) == 0)
        {
            printf("Error : you cannot modify an existing alias\n");
            free(node->old);
            free(node->old_new);
            free(node);
            return;
        }
        prev=curr;
        curr = curr->next;
    }
    prev->next=node;
}

int remove_alias(char *old_new)
{
    alias *curr = head_alias;
    alias *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(curr->old_new, old_new) == 0)
        {
            if (head_alias == curr)
            {
                head_alias = head_alias->next;
                free(curr->old);
                free(curr->old_new);
                free(curr);
                return 1;
            }
            prev->next = curr->next;
            free(curr->old);
            free(curr->old_new);
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

char *get_alias(char *old_new)
{
    alias *curr = head_alias;
    while (curr != NULL)
    {
        if (strcmp(curr->old_new, old_new) == 0)
        {
            return curr->old;
        }
        curr = curr->next;
    }
    return old_new;
}

void print_alias()
{
    if (head_alias == NULL)
    {
        printf("No aliases\n");
        return;
    }
    alias *curr = head_alias;
    while (curr != NULL)
    {
        printf("%s -> %s\n", curr->old_new, curr->old);
        curr = curr->next;
    }
    return;
}
