#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_TOKEN_LENGTH 50
#define MAX_TOKEN_COUNT 100
#define MAX_LINE_LENGTH 512

// Simple implementation for Shell command
// Assume all arguments are seperated by space
// Erros are ignored when executing fgets(), fork(), and waitpid(). 

/**
 * Sample session
 *  shell: echo hello
 *   hello
 *   shell: ls
 *   Makefile  simple  simple_shell.c
 *   shell: exit
**/

void runcommand(char* command, char** args, int count) {
//before fork separate via some kind of tokenizing on <, >, and |
//take what is on the right side of delineator's output into what is on the left
  printf("Command is %s", command);
  
  int size = sizeof(*args)/sizeof(args[0]);
  printf("Size %d", count);
  //for (int i = 0; i<size; i++){
    printf("\n Args are %s \n" , args[1]);
  //}
  
  pid_t pid = fork();
  if(pid) { // parent
    	waitpid(pid, NULL, 0);
  } else { // child
    	execvp(command, args);
  }
}
//Based on Nik's implementation handles ctrl+z
void ctrlz(int sig){
    static int count = 0;
    count++;
    if (count == 1)
        signal(SIGTSTP, SIG_DFL);
}


int main(){
    signal(SIGTSTP, ctrlz);
    char line[MAX_LINE_LENGTH];
    //printf("shell: "); 
    while(fgets(line, MAX_LINE_LENGTH, stdin)) {
    	// Build the command and arguments, using execv conventions.
    	line[strlen(line)-1] = '\0'; // get rid of the new line
    	char* command = NULL;
    	char* arguments[MAX_TOKEN_COUNT];
    	int argument_count = 0;

    	char* token = strtok(line, " ");
    	while(token) {
      		if(!command) command = token;
      		arguments[argument_count] = token;
	      	argument_count++;
      		token = strtok(NULL, " ");
    	}
    	arguments[argument_count] = NULL;
	if(argument_count>0){
		if (strcmp(arguments[0], "exit") == 0)
            		exit(0);
    		runcommand(command, arguments, argument_count);
	}
        //printf("shell: "); 
    }
    return 0;
}
