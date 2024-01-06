/* Name: Tia Mehta
Class: CMSC257
Files: shellex.c is a custom shell script
Date: 11/30/23
*/

/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
// This declares the sigint handler, and will do nothing if sigint is signaled
void sigint_handler(int sig){
    
   }

int main( int argc, char** argv) 
{
    char cmdline[MAXLINE]; /* Command line */

   signal(SIGINT, sigint_handler);

   

    while (1) {
	/* Read */
    //custom prompt if -p is option is used
    if(argc == 3 && !strcmp(argv[1], "-p")){
        printf("%s>", argv[2]);
    }else{
        printf("sh257>");
    }                  
	Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
        if ((pid = Fork()) == 0) {   /* Child runs user job */
            if (execvp(argv[0], argv) < 0) {
                printf("Execution failed (in fork)\n");
                printf("%s: Command not found.\n", argv[0]);
                exit(1);
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
		unix_error("waitfg: waitpid error");
        printf("Process exited with status code %d\n", WEXITSTATUS(status));
	}
	else
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    if (!strcmp(argv[0], "exit"))
    raise(SIGTERM);
//prints the process id
    if (!strcmp(argv[0], "pid")){
        pid_t pid = getpid();
        printf("pid:%d\n", pid);
        return 1;
    }
    //prints the parent process id
    if (!strcmp(argv[0], "ppid")){
        pid_t ppid = getppid();
        printf("ppid:%d\n", ppid);
        return 1;
    }
    //Help command that prints more information
    if (!strcmp(argv[0], "help")){
        printf("\n\n*************************************************************************************\n");
        printf(" A Custom Shell \n");
        printf("    - Devloped by Tia Mehta \n");
        printf("*************************************************************************************\n");
        printf(" Usage: \n");
        printf("    - To change prompt type -p <new prompt name> \n");
        printf(" BUILTIN COMMANDS: \n\n");
        printf("    - exit:  Exits shell\n");
        printf("    - pid:   Prints process id of shell\n");
        printf("    - ppid:  Prints parent process id of shell\n");
        printf("    - cd:    Prints current working directory\n");
        printf("    - cd <path>:    Changes current working directory to specified path\n\n");
        printf(" SYSTEM COMMANDS: \n\n");
        printf("For all other commands please refer to man pages\n");
        printf("*************************************************************************************\n");
        return 1;

    }
    // cd command to change directory or show the current working directory
    if (!strcmp(argv[0], "cd")){
        if (argv[1] == NULL){
            char cwd[MAXLINE];

            if(getcwd(cwd, sizeof(cwd))!=NULL){
                printf("%s\n", cwd);
                return 1;
            }else{
                printf("Error retreiving current working directory.\n");
                return 1;
            }
   
        }else{
            if(chdir(argv[1]) !=0){
                printf("Error changing current working directory\n");
            }
            return 1;
        }
    }  
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;  
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


