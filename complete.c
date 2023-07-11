#include "header.h"

complete *init_complete()
{
    complete *temp = (complete *)malloc(sizeof(complete));
    temp->next = NULL;
    temp->string = (char *)malloc(sizeof(char) * 1024);
    return temp;
}

void add_complete(char *final)
{
    complete *node = init_complete();
    strcpy(node->string, final);
    complete *curr = head_complete;
    if (head_complete == NULL)
    {
        head_complete = node;
        return;
    }
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = node;
    return;
}

void print_complete()
{
    complete *curr = head_complete;
    while (curr != NULL)
    {
        printf("\033[1;33m%s\033[0m\n", curr->string);
        curr = curr->next;
    }
    curr = head_complete;
    while(head_complete!=NULL)
    {
        head_complete=curr->next;
        free(curr->string);
        free(curr);
        curr=head_complete;
    }
}