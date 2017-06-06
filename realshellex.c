/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
int argc;

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
	/* Read */
	printf("> ");                   
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
    pid_t pid[64];           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */
    int i; 
	if (!builtin_command(argv)) { 
        for(i=0;i<argc;i++){
			if ((pid[i] = Fork()) == 0) {   /* Child runs user job */
				if (execve(argv[i], argv, environ) < 0) {
               		printf("%s: Command not found.\n", argv[0]);
               		exit(0);
            	}
        	}
		}

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid[i], &status, 0) < 0)
		unix_error("waitfg: waitpid error");
	}
	else{
		for(i=0;i<argc;i++)
	    printf("%d %s\n", pid[i], argv[i]);
		}
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")){ /* quit command */
		kill(0, SIGKILL);
		exit(0);  
	}
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
		return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    //int argc;            /* Number of args */
    int bg;              /* Background job? */
	
	buf[strlen(buf)-1] = ' ';
	bg = buf[strlen(buf)-2] == '&';
    /* Build the argv list */
    argc = 0;
 	argv[argc++] = strtok(buf, " &");
	while(argv[argc-1] != NULL)
		argv[argc++] = strtok(NULL, " &");
	argc--;
    
    if (argc == 0)  /* Ignore blank line */
		return 1;
    /* Should the job run in the background? */
    //if ((bg = (*argv[argc-1] == '&')) != 0){
	//	argv[--argc] = NULL;
	//}
		
    return bg;
}
/* $end parseline */

