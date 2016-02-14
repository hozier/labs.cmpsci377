#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void process_termination(pid_t pid){ kill(pid, SIGKILL); }

// overview: prompt the user to enter a process id and then
// use the kill system call to terminate it.
void interface(){
  int response = 0;
  // pid_t pid;

  printf("%s: ","Enter a process id [options -- an integer]");
  scanf("%d", &response);
  printf("%s\n\n", "Attempting kill...");

  pid_t pid = (pid_t)response;
  process_termination(pid/*some pid from the command line*/);
}


int main(int argc, char const *argv[]) {
  /* code */
  // overview: runs the kill routine.
  interface();
  return 0;
}
