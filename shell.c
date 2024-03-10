/*****
* Project 04: Shell
* COSC 208, Introduction to Computer Systems, Fall 2023
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROMPT "shell> "

//bool flag for if bg process is running
int bgProcess;

/*
 * Break input string into an array of strings.
 * @param input the string to tokenize
 * @param delimiters the characters that delimite tokens
 * @return the array of strings with the last element of the array set to NULL
 */
char** tokenize(const char *input, const char *delimiters) {    
    char *token = NULL;

    // make a copy of the input string, because strtok
    // likes to mangle strings.  
    char *input_copy = strdup(input);

    // find out exactly how many tokens we have
    int count = 0;
    for (token = strtok(input_copy, delimiters); token; 
            token = strtok(NULL, delimiters)) {
        count++ ;
    }
    free(input_copy);

    input_copy = strdup(input);

    // allocate the array of char *'s, with one additional
    char **array = (char **)malloc(sizeof(char *)*(count+1));
    int i = 0;
    for (token = strtok(input_copy, delimiters); token;
            token = strtok(NULL, delimiters)) {
        array[i] = strdup(token);
        i++;
    }
    array[i] = NULL;
    free(input_copy);
    return array;
}

/*
 * Free all memory used to store an array of tokens.
 * @param tokens the array of tokens to free
 */
void free_tokens(char **tokens) {    
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

void sigchld_handler(int signo) {//assisted by ChatGPT
    int status;
    if (waitpid(bgProcess, &status, WNOHANG) > 0) {
        if (bgProcess != -1)//error handling for clean prints, beyond scope of assignment so patch fix
            printf("Background process completed. PID: %d\n", bgProcess);
        bgProcess = -1;  //resets background process
    }
}


int main(int argc, char **argv) {    
    
    bgProcess = -1;
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, SIG_IGN);    //control c handling assisted by ChatGPT


    printf("%s", PROMPT);
    fflush(stdout);  // Display the prompt immediately
    char buffer[1024];
    while (fgets(buffer, 1024, stdin) != NULL) {
        char **command = tokenize(buffer, " \t\n");
        
        if (command[0] == NULL  || command[0][0] == '#') {
            // Do nothing
        
        } else if (strcmp(command[0], "cd") == 0) {
            // Handle 'cd' command
            if (command[1]) {
                if (chdir(command[1]) != 0) {   //chdir assisted by ChatGPT
                    perror("chdir");
                }
            }
        } else if (strcmp(command[0], "fg") == 0) {
            // Handle 'fg' command
            if (bgProcess != -1) {
                // Continue the background process
                kill(bgProcess, SIGCONT);   //kill and sigcont assisted by ChatGPT
                int status;
                waitpid(bgProcess, &status, 0);  // Wait for it to finish
                bgProcess = -1;
            }
        
        } else {
            int size = 0;
            //bool if command should be in bg
            int bgCommand = 0;
            
            //count number of arguments passed
            while (command[size] != NULL) {
                size++;
            }
            //process is bg, set values accordingly
            if (*command[size-1] == '&'){   
                bgCommand = 1;
                //remove '&' for running
                command[size-1] = NULL;    //could be command[--size] = NULL but if it ain't broke...
                size -= 1;  
            } 
            int pid = fork();

            //child process
            if (pid == 0) {
                signal(SIGINT, SIG_DFL);    //ChatGPT (for signal)
                //run command
                if (execvp(command[0], command) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            
            //parent process
            } else if (pid > 0){
                if (!bgCommand) {
                    int status;
                    //wait for child if not bg
                    waitpid(pid,&status,0);
                    
                } else {
                    //let bg process run, move on to next command
                    if (bgProcess != -1) {
                        printf("not started: background process already running\n");
                    } else {
                        bgProcess = pid;
                        printf("BG Process Started. PID: %d\n", pid);
                    }
                }
            //error handling
            } else {
                perror("Fork Failed");
            }
        }
        //prevent leakage
        free_tokens(command);
        //reprompt and repeat
        printf("%s", PROMPT);
        fflush(stdout);  // Display the prompt immediately
    }
    printf("\nexit\n");
    return EXIT_SUCCESS;
}
