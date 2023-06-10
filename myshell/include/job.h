#ifndef JOB_H
#define JOB_H
#include "csapp.h"
#include <stdbool.h>

#define MAXJOBS 16
#define BG 1
#define FG 2
#define MAXLINE 8192

// Forward declaration
struct Job;

typedef struct Job {
    pid_t pid;
    int jid;
    bool stopped; /* Stopped | Running */
    bool signaled; /* If stopped by a signal but not notified (uncaught signal)*/
    bool done; /* Not Started | Completed */
    struct Job* next;
    char cmdline[MAXLINE];
} Job;

extern Job* head_jobs;

Job* Newjob(pid_t pid, int jid, char cmdline[MAXLINE]);
Job* Getjobwjid(int jid);
Job* Getjobwpid(int pid);
void Cleanjobs();
void Waitjob(Job* job);

#endif