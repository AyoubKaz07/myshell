#include "csapp.h"
#include "job.h"


Job* head_jobs = NULL;

Job* Newjob(pid_t pid, int jid, char cmdline[MAXLINE]) {
    Job* job = malloc(sizeof(Job));
    if (!job) {
        printf("Cannot allocate more memory");
    }

    job->next = NULL;
    job->pid = pid;
    job->jid = jid;
    job->stopped = false;
    job->signaled = false;
    job->done = false;
    strcpy(job->cmdline, cmdline);
    if (!head_jobs) {
        head_jobs = job;
    }
    else {
        Job* iter = head_jobs;
        while (iter->next) {
            iter = iter->next;
        }
        iter->next = job;
    }
    return job;
}

Job* Getjobwjid(int jid) {
    Job* iter = head_jobs;
    while (iter) {
        if (iter->jid == jid) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

Job* Getjobwpid(int pid) {
    Job* iter = head_jobs;
    while (iter) {
        if (iter->pid == pid) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

void DeleteJob(pid_t pid) {
    Job* iter = head_jobs;
    if (!iter) {
        return ;
    }
    if (iter->pid == pid){
        head_jobs = NULL;
        free(iter);
        return;
    }
    while (iter->next){
        if (iter->next->pid == pid){
            iter->next = iter->next->next;
            return;
        }
        iter = iter->next;
    }
    printf("No Job with pid: %d \n", pid);
    return;
}

void Cleanjobs() {
    int status;
    pid_t pid;
    /*  WUNTRACED Checks both terminated and stopped processes,
     ** WNOHANG Returns (don't wait) in case no child has terminated yet (do something useful instead)
    */
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED); 
    while (pid > 0) {
        Job* job = Getjobwpid(pid);
        if (WIFSTOPPED(status)){
            printf("Job %d Stopped \n", job->pid);
            job->stopped = 1;
        }
        else {
            if (WIFSIGNALED(status)){
                printf("Job %d terminated by signal here: %d \n", pid, WTERMSIG(status));
            }
            else {
                printf("Job %d terminated normally \n", pid);
            }
            job->done = 1;
        } 
        pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    }

    Job* iter = head_jobs;
    while (iter){
        if (iter->done){
            DeleteJob(iter->pid);
        }
        else if (iter->stopped && !iter->signaled){
            iter->signaled = 1;
        }
        iter = iter->next;
    }
}

void Waitjob(Job* job){
    int status;
    if (waitpid(job->pid, &status, 0) > 0){
        if (WIFSTOPPED(status)){
            job->stopped= 1;
            printf("Job with pid: %d Stopped by signal %d \n", job->pid, WSTOPSIG(status));
            return;
        }
        else if (WIFSIGNALED(status)) {
            job->signaled = 1;
            printf("Job with pid: %d Terminated by signal %d \n", job->pid, WTERMSIG(status));
        }
        else if (!WIFEXITED(status)){
            printf("Job with pid %d Exited abnormally", job->pid);
        }
        DeleteJob(job->pid);
    }
    else {
        unix_error("weird waitpid error"); 
    } 
    return;
}