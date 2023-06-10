# myshell
Developing a basic Shell (myshell) Exploring exceptional control flow and job management through a shell application developed based on Chapter 8 of "Intro to Computer Systems" (15-213) textbook. Covered processes, signals, and job control. Not an excellent error checking (to be continued ...)


8.26 ◆◆◆◆
Using the example in Figure 8.23 as a starting point, write a shell program that
supports job control. Your shell should have the following features:

- The command line typed by the user consists of a name and zero or more argu-
ments, all separated by one or more spaces. If name is a built-in command, theshell handles it immediately and waits for the next command line. Otherwise,
the shell assumes that name is an executable file, which it loads and runs in the
context of an initial child process (job). The process group ID for the job is
identical to the PID of the child.

- Each job is identified by either a process ID (PID) or a job ID (JID), which
is a small arbitrary positive integer assigned by the shell. JIDs are denoted on
the command line by the prefix ‘%’. For example, ‘%5’ denotes JID 5, and ‘5’
denotes PID 5.

- If the command line ends with an ampersand, then the shell runs the job in
the background. Otherwise, the shell runs the job in the foreground.
Typing Ctrl+C (Ctrl+Z) causes the kernel to send a SIGINT (SIGTSTP) signal
to your shell, which then forwards it to every process in the foreground process
group.

- The "jobs" built-in command lists all background jobs.
- The "bg job" built-in command restarts job by sending it a SIGCONT signal
and then runs it in the background. The job argument can be either a PID or
a JID.

- The "fg job" built-in command restarts job by sending it a SIGCONT signal and
then runs it in the foreground.

- The shell reaps all of its zombie children. If any job terminates because it
receives a signal that was not caught, then the shell prints a message to the
terminal with the job’s PID and a description of the offending signal.
