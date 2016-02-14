#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

void process_termination(pid_t pid){ kill(pid, SIGKILL); }

// overview: prompt the user to enter a process id and then
// use the kill system call to terminate it.
void interface(){
  char response;
  // pid_t pid;
  while (1/* condition */) {
    /* code */
    // read a string, if string is an integer, proceed; else exit 
    printf("%s: ","Enter a process id [options -- an integer | -- a noninteger to exit ]");
    scanf("%s", &response);

    if (isdigit(response)/* condition */) {
      /* code */
      printf("Killing... %d\n\n", response);
      pid_t pid = (pid_t)(atoi(&response));
      process_termination(pid/*some pid from the command line*/);
    } else { printf("%s\n", "exiting..."); break; }

  }

}


int main(int argc, char const *argv[]) {
  /* code */
  // overview: runs the kill routine.
  interface();
  return 0;
}
