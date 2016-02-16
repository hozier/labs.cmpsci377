#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

// overview: kills process
void process_termination(pid_t pid){
  /* if kill is successful print completed, else print unable to find pid */
  if (kill(pid, SIGKILL) == 0) { printf("[Process %d completed]\n\n", pid); } else{  printf("process id unknown\n\n"); }
}

// overview: prompt the user to enter a process id and then
// use the kill system call to terminate it.
void interface(){
  char response;
  // pid_t pid;
  printf("%s\n\n","Enter a process id\noptions [-- an integer to represent a pid] [-- any noninteger character to exit ]");
  while (1/* condition */) {
    /* code */
    // read a string, if string is an integer, proceed; else exit
    printf("%s","process_termination> ");
    scanf("%s", &response);

    if (isdigit(response)/* condition */) {
      /* code */
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
