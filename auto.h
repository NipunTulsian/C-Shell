#ifndef __AUTO_
#define __AUTO_

void die(const char *s);
void disableRawMode();
void enableRawMode();
char *auto_complete(char *input, int *matched,char**  matched_name);

#endif