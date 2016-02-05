#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void child_make_child(){
  pid_t process_creation2 = fork();
  int status2;

  if(process_creation2 == 0){
    // overview: we are in grandchild
    // sleep the process
    sleep(10);

    // overview: gets and prints pid of most current child.
    printf("Grandchild process with process id [%d] has completed.\n", getpid());

    // overview: perform normal program termination
    // exit second child created.
    _Exit(0);

  }
  else{
    // code executed by parent below:
    // overview: we are in secondly created parent branch
    // otherwise known as the first child created from first
    // fork call

    // wait/listen for child to exit
    // api options: WUNTRACED:
    // Also return if a child has stopped. Status for traced children which
    // have stopped is provided even if this option is not specified.
    waitpid(process_creation2, &status2, WUNTRACED);
    printf("Child process with process ID [%d] has completed.\n", getpid());

    // exit first child created.
    _Exit(0);
  }
}

int main(int argc, char const *argv[]) {
  /* code */
  pid_t p;
  pid_t process_creation1 = fork();

  int status1;
  if(process_creation1 == 0){
    // overview: we are in child
    child_make_child();

  } else{
    // overview: we are in parent
    // AKA GRANDPARENT
    waitpid(process_creation1, &status1, WUNTRACED);
    printf("Parent process with process ID [%d] has finished\n", getpid());
  }

  return 0;
}
