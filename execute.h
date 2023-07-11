#ifndef __EXECUTE_
#define __EXECUTE_
int tokenise_com(char *input, char **list);
void before_execute(char* input);
void reset_IO();
void execute(char **arg, int num_arg);

#endif