#include "header.h"

struct dirent **dir_auto;

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

int min(int a, int b)
{
    return a < b ? a : b;
}

char *auto_complete(char *input, int *matched, char **match_name)
{
    int len = strlen(input);
    int k = len - 1;
    for (; k >= 0; k--)
    {
        if (input[k] == '/')
            break;
    }
    char *path = (char *)malloc(sizeof(char) * 1024);
    memset(path, '\0', 1024);
    sprintf(path, "./");
    for (int i = 0; i < k; i++)
    {
        path[i + 2] = input[i];
    }
    len = strlen(&input[k + 1]);
    int num = scandir(path, &dir_auto, NULL, alphasort);
    int count = 0;
    for (int i = 0; i < num; i++)
    {
        if (strncmp(&input[k + 1], dir_auto[i]->d_name, len) == 0)
        {
            match_name[count] = (char *)malloc(sizeof(char) * 1024);
            memset(match_name[count], '\0', 1024);
            strcpy(match_name[count++], dir_auto[i]->d_name);
        }
    }
    char *temp = (char *)malloc(sizeof(char) * 1024);
    memset(temp, '\0', 1024);
    *matched = count;
    if (count == 0)
    {
        return NULL;
    }
    else
    {
        if (count > 1)
        {
            printf("\n");
            for (int i = 0; i < count; i++)
            {
                char* name=(char*)malloc(sizeof(char)*1024);
                memset(name,'\0',1024);
                struct stat fd;
                char path2[1024];
                strcpy(path2, path);
                strcat(path2, "/");
                strcat(path2, match_name[i]);
                stat(path2, &fd);
                strcpy(name,match_name[i]);
                if (S_ISDIR(fd.st_mode))
                {
                    name[strlen(match_name[i])] = '/';
                }
                else if (S_ISREG(fd.st_mode))
                {
                    name[strlen(match_name[i])] = ' ';
                }
                for (int j = 0; j < strlen(name); j++)
                {
                    printf("%c", name[j]);
                }
                printf("\n");
            }
        }
        int min_len = 1e9;
        for (int i = 0; i < count; i++)
        {
            min_len = min(min_len, strlen(match_name[i]));
        }

        int len_temp = 0;
        for (int i = 0; i < min_len; i++)
        {
            char c = match_name[0][len + i];
            for (int j = 0; j < count; j++)
            {
                if (match_name[j][len + i] != c)
                {
                    if (len_temp)
                        return temp;
                    else
                        return NULL;
                }
            }
            temp[len_temp++] = c;
        }
    }
    if (temp)
    {
        return temp;
    }
    return NULL;
}