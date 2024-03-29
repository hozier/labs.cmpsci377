#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#define SIZE_LIMIT 100

// overview: parse argurments recieved from stdin and store them into an array of strings.
char** parse_commands(char *argStorage, int *counter){
  char** commandStorage = (char**)malloc(SIZE_LIMIT*sizeof(char)); //array to store commands once input is seperated by " "
  char* command; // single command pointer
  char* current = " "; //used to detect a space in the input and will be used in strtok to seperate commands

  //split commands using the space
  command = strtok(argStorage,current); //split argStorage into commands seperated by 'current' which is a " "
  while (command!=NULL) { //iterates through all existing arguments
    commandStorage[*counter] = command;  //store each commands
    command = strtok(NULL,current); //used to detect when no more commands are in the input. this ends the while loop
    (*counter)++;//increase index for next command
  }


  if(strcmp(commandStorage[0],"quit\n") == 0){ //detects the command quit in order to exit the program
      exit(0);
  }
  return commandStorage;
}

// overview: append parsed arguments to the current working directory and execute commands.
void execute_commands(char *argStorage, char **commandStorage, char *bin, int *counter){
  size_t j;
  for(j=0; j<*counter; j++){ //for loop to actually execute all the commands once their seperated
    if(fork()==0){ //if child
      //get command
      strcat(bin, "/");
      strcat(bin,commandStorage[j]); //strcat appends a copy of the command at index j to path
      printf("Command to execute: %s\n", bin);
      strtok(bin,"\n"); //delete \n in path
      //execute the command
      int exec = execl(bin,argStorage,NULL); //execl means execute and leave, meaning a process will be executed and then terminated when done
                                             //bin is path where commands stand while argStorage points to commands identical to those in the path
      if(exec < 0){ //if input is incorrect
        printf("Invalid input. To quit, enter \'quit\'.\n");
        exit(0);
      }
    }
    waitpid(-1,NULL,0); //wait for children to be done
  }
}


int main(){
  int counter; //initialize counter for index in commandStorage to input different commands once seperated
  char argStorage[SIZE_LIMIT]; //array to store user's input as arguments
  char bin[SIZE_LIMIT]; //path
  getcwd(bin, sizeof(bin)); //gets current directory, assigns it to bin
  printf("Working directory is: %s\n", bin);

  while (1) { //while more arguments exist
    counter = 0;
    printf("batch-shell> ");
    fgets(argStorage,SIZE_LIMIT,stdin);
    /* -fgets reads characters from stdin and stores them in our argStorage up to 99 arguments
        until theres no more
       -stdin can be used as an argument for any function that expects an input stream.
        this includes the keyboard in which our shells will be typed. stdin = standard input stream */

    if(feof(stdin)){ break; } // if EOF discovered, exit program.

    char **commandStorage = parse_commands(argStorage, &counter); // parse argurments recieved from stdin and store them into an array of strings.
    execute_commands(argStorage, commandStorage, bin, &counter); // append parsed arguments to the current working directory and execute commands.

  }
  return 0; //while loop is over and so is program
}
