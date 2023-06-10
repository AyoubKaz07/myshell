#include "csapp.h"
#include "job.h"

int builtin_command(char** argv);
int ListJobs();
int BackgroundCommand(char** argv);
int ForgroundCommand(char** argv);


int builtin_command(char** argv) {
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }
    if (!strcmp(argv[0], "&")) {
        return 1;
    }
    if (!strcmp(argv[0], "fg")) {
        return ForgroundCommand(argv);
    }
    if (!strcmp(argv[0], "bg")) {
        return BackgroundCommand(argv);
    }
    if (!strcmp(argv[0], "jobs")) {
        return ListJobs();
    }
    return 0;
}

int ListJobs() {
    Job* iter = head_jobs;
    while (iter) {
        printf("[%d] %d %s   %s", iter->jid, iter->pid, iter->done ? "Stopped" : "Running", iter->cmdline);
        iter = iter->next;
    }
    return 1;
}

int BackgroundCommand(char** argv) {
    /* CASE: "bg %jid" */
    Job* job = NULL;
    if (argv[1][0] == '%'){
        int jid = atoi(argv[1]+1); /* argv[1]+1 points at jid not '%' */
        job = Getjobwjid(jid);
    }
    else {
        int pid = atoi(argv[1]);
        job = Getjobwpid(pid);
    }

    if (!job){
        printf("Usage... \n");
    }
    Kill(job->pid, SIGCONT);
    job->stopped = 0;
    /*Leave it in Background*/
    return 1;
}

int ForgroundCommand(char** argv) {
    /* CASE: "bg %jid" */
    Job* job = NULL;
    if (argv[1][0] == '%'){
        int jid = atoi(argv[1]+1); /* argv[1]+1 points at jid not '%' */
        job = Getjobwjid(jid);
    }
    else {
        int pid = atoi(argv[1]);
        job = Getjobwpid(pid);
    }

    if (!job){
        printf("Usage... \n");
        return 1;
    }
    Kill(job->pid, SIGCONT);
    job->stopped = 0;

    /* Run in Forground*/
    Waitjob(job);
    return 1;
}