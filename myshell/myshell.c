/* $begin shellmain */
#include "csapp.h"
#include "job.h"
#include "builtin.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);

pid_t pid;

void sigint_handler(int sig) {
    printf("HERE !!!!");
    if (!kill(-pid, 0))/* Checks existence */
        Kill(-pid, sig);
}


int main() {
    Signal(SIGINT, SIG_IGN);
    Signal(SIGTSTP, SIG_IGN);
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
        /* Read */
        printf("> ");                   
        Fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);
        Cleanjobs();
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
    int jid;             /* Job id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
    return;   /* Ignore empty lines */
    
    Signal(SIGINT, sigint_handler);
    Signal(SIGTSTP, sigint_handler);
    if (!builtin_command(argv)) { 
        /* Install Signal Handlers */
        if ((pid = Fork()) == 0) {   /* Child runs user job */
            setpgid(pid, 0);
            if (execvp(argv[0], argv) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
        jid++;
        Job* job = Newjob(pid, jid, cmdline);
        
        /* Parent waits for its child to terminate (forground) */
        /* No need to update status */
        if (!bg) {
            Waitjob(job);
        }
        /* Running in Background */
        else {
            printf("[%d] %d %s", job->jid, job->pid, job->cmdline);
        }
    }
    Signal(SIGINT, SIG_IGN);
    Signal(SIGTSTP, SIG_IGN);
    return;
}


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