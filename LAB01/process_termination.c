#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void process_termination(pid_t pid){ kill(pid, SIGINT); }

int main(int argc, char const *argv[]) {
  /* code */
  const char *user_cli_input = argv[1];
  pid_t pid = atoi(user_cli_input);
  printf("%s%d...\n", "Killing ", pid);

  // TODO: prompt the user to enter a process id and then
  // use the kill system call to terminate it.
  process_termination(pid/*some pid from the command line*/);
  return 0;
}
