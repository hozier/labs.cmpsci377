Paul Fretz
Philron Hozier
Ethan Miller
CMPSCI 377 Operating Systems 
Lab 01 Processes 
February 15, 2016

/* write a design document that documents your design choices */
Part 1: Process Creation

Our design choice for part one revolved around abstracting the task of having the initial child process create a child. 
The inclusion of abstraction allowed us to view the solution in only two parts within the main function: 
if the pid returns zero -- do something with the initial child process, otherwise wait for the initial child’s death and print. 
We chose to implement a helper routine void child_make_child() which bundled the logic of the initial child process creating a 
new process for the sake of simplifying the problem.

The basic algorithm for child_make_child() is as follows:
    fork a new process
    if return value of fork is zero
    /* we are in the child’s process child */
        sleep
        print process id of the child process’s child (otherwise known as grandchild)
        terminate process
    else
        wait for child of child process to die 
        print child process id 
        terminate process

The else case within the main function simply waits for the death of the initial child process. 
Once the wait stops, the program prints a statement and exits.


Part 2: Process Termination

The structure of part two’s code is best explained through two levels of abstraction. In words, we have one helper method calling another 
more specialized helper method. The more specialized helper method void process_termination(pid_t pid) executes kill system calls on supplied process ids. 
Interface can be thought of as an entry point for our process termination program. void interface()takes care of user prompts, error handling and program exiting logic.  
Once we have checked the correctness of a user supplied pid and its validity, it is then passed off to void process_termination(pid_t pid) which executes the kill system call. 
If and only if the kill routine returns 0, then we have successfully killed the pid supplied by the user.

Part 3: Batch Scheduler
Most of the batch scheduler is implemented within a while loop that continuously takes arguments in from the user. Multiple programs can be entered by the user at once to then be executed sequentially, as per the documentation. 
For demonstration purposes, our batch scheduler can execute the other two executables from this assignment, as they are all located in the same directory.

Notable system calls used for this part of the assignment include execl(), fork() and waitpid(). The first executes another program while the calling program is still running. The latter two manage child processes 
- i.e., the batch scheduler waits for its children to finish execution before prompting the user for more commands. Each child process is tasked with executing one of the programs specified by the user. 
