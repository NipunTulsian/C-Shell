# <b>Custom C Shell</b>
A basic shell made as a part of Operating Systems and Networks course.
<hr>

## Nipun Tulsian <b>| 2021101055</b>
<hr>

# <b>Functionality</b>
- Prompt with username, hostname and directory
- Basic commands : cd, pwd, echo, ls, discover, pinfo, fg, bg, sig, I/O Redirection, piping, autocompletion, signal handling
- Ability to run external commands as foreground and background
<hr>

# <b>Installation</b>
1. Run <mark style="background-color: grey; border-radius: 5px; border-">make</mark> to build the executable
2. Run <mark style="background-color: grey; border-radius: 5px"> ./a.out</mark> to start the shell
# <b>File & Functions</b>
## <b>Supporting Files</b>
#### Files containing functions required for proper functioning of the shell.

- <mark style="background-color: grey; border-radius: 5px">main.c</mark>
> Run shell loop

> Extract command from input and tokenise them based on ; and  &

> Checks for terminated child processes

> Handles I/O redirection

> Have customized signal handlers for various signals
- <mark style="background-color: grey; border-radius: 5px">prompt.c</mark>
> Print prompt

- <mark style="background-color: grey; border-radius: 5px">execute.c</mark>
> Tokenise the input furthur and store them as words

> According to command to execute redirect the tokenised input to respective functions

- <mark style="background-color: grey; border-radius: 5px">history.c</mark>
> Adds command to history

> Stores maximum of 20 commands

> Prints the last 10 commands

- <mark style="background-color: grey; border-radius: 5px">alias.c</mark>
> Stores aliases

- <mark style="background-color: grey; border-radius: 5px">complete.c</mark>
> Stores exit status of terminated background process

## <b>Command Files</b>
- <mark style="background-color: grey; border-radius: 5px">pwd.c</mark>
> Prints the working directory
- <mark style="background-color: grey; border-radius: 5px">echo.c</mark>
> Prints user input after removing excess spaces/tabs

- <mark style="background-color: grey; border-radius: 5px">ls.c</mark>
> Prints all files/sub-directories in a location

> Flags implemented: a, l, al, la

> Multiple directories supported

- <mark style="background-color: grey; border-radius: 5px">cd.c</mark>
> Navigate to different directories

- <mark style="background-color: grey; border-radius: 5px">pinfo.c</mark>
> Prints info about process with given PID (current process if none is given)

- <mark style="background-color: grey; border-radius: 5px">discover.c</mark>
> Emulates the basics of the find command

> Flags implemented : d,f

> If finds target file recursively within target directory(if mentioned else current directory) and printd its path

> Also finds hidden files

- <mark style="background-color: grey; border-radius: 5px">jobs.c</mark>
> Forks the parent process and runs command of child in child process created

> In parent if the child process is foreground waits for the process to terminate else if child is background adds the name and id of child process to list
<hr>

- <mark style="background-color: grey; border-radius: 5px">auto.c</mark>
> Functions to enable and disable rawmode

> When pressed tab list all files matching with given name in mrntioned path or current directory and autofills the lagest substring matching

- <mark style="background-color: grey; border-radius: 5px">bg_jobs.c</mark>
> fg - Brings the running or stopped background job corresponding to job number to the foreground, and changes
its state to running. The shell throw an error if no job with the given job number exists.

> bg - Changes the state of a stopped background job to running (in the background). The shell should throw an
error if no background job corresponding to the given job number exists, and do nothing if the job is already
running in the background.

> sig - Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to the
signal number to that process. The shell throw an error if no job with the given number exists.

> jobs - This command prints a list of all currently running background processes spawned by the shell in
alphabetical order of the command name, along with their job number (a sequential number assigned by your
shell), process ID, and their state, which can either be running or stopped.
If the flag specified is -r, then print only the running processes else if the flag is -s then print the stopped
processes only.

# <b> Signal Handling </b> #
> CTRL-C : It should interrupt any currently running foreground job, by sending it the SIGINT signal. This have no effect on the shell if there is no foreground process running.

> CTRL-Z It should push any currently running foreground job into the background, and change its state from ‘running’ to ‘stopped.’ This have no effect on the shell if there is no foreground process running.

> CTRL-D It should log you out of your shell, without having any effect on the actual terminal.
# <b>Assumptions</b>

- Reasonable assumptions are made for things like length of current path, number of commands, command length, etc. The shell will work perfectly for all "day to day work".

- File and directory names shouldn't contain spaces or special characters.

- <mark style="background-color: grey; border-radius: 5px">pinfo</mark> may break on systems not running Ubuntu. This command uses <mark style="background-color: grey; border-radius: 5px">/proc</mark> files which may not be same for other OS.

- Every user input will be stored as it is in history, given it is not same as the last input and contains at least one character other than space or tab or enter.

- Prompt will display only terminated normally or terminated abnormally when a background process ends.

- Prompt will check exist status of background processes only after user inputs a command (possibly empty). The same happens in actual Bash shell.

- Any error while running a command will result in aborting the command after printing an error message.

- <mark style="background-color: grey; border-radius: 5px">&</mark> (used for running process in background) can come anywhere in command.

- A hidden file <mark style="background-color: grey; border-radius: 5px">(.history)</mark> is used to store history in a readable format. Any changes explicitly done in this file may result in unexpected output of <mark style="background-color: grey; border-radius: 5px">history</mark> command.










