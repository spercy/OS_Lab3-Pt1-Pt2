//#include <stdio.h>
/*#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers". In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */
 /*
int main(int argc, char **argv)
{
 int pipefd[2];
 int pipefd2[2];
 int pid,pid2;

 char *cat_args[] = {"cat", "scores", NULL};
 char *grep_args[] = {"grep", NULL, NULL};
 char *sort_args[] = {"sort", NULL, NULL};

 // make a pipe (fds go in pipefd[0] and pipefd[1])
if(pipe(pipefd) == -1){
  fprintf(stderr, "The Pipe1 Failed\n" );
  return 1;
 }
if(pipe(pipefd2) == -1){
  fprintf(stderr, "The Pipe2 Failed" );
  return 1;
 }

pid = fork();

 if(argc<1){
  printf("Please enter an argument for grep\n");
 }
 grep_args[1] = argv[1];
 if (pid == 0){

  pid2 = fork();

  if(pid2 == 0){
    dup2(pipefd2[0], 0);


    close(pipefd[0]);
      close(pipefd[1]);
      close(pipefd2[0]);
      close(pipefd2[1]);
    execvp("sort", sort_args);
 }
 else if(pid2<0){
  perror("error");
 }

 else{
 // child gets here and handles "grep Villanova"
 // replace standard input with input part of pipe

  dup2(pipefd[0], 0);
  dup2(pipefd2[1], 1);

 // close unused hald of pipe

  close(pipefd[0]);
    close(pipefd[1]);

    close(pipefd2[0]);
    close(pipefd2[1]);
    // execute grep
  execvp("grep", grep_args);
 }

 }

else{
 // parent gets here and handles "cat scores"
 // replace standard output with output part of pipe
  dup2(pipefd[1], 1);
 // close unused unput half of pipe
  close(pipefd[0]);
    close(pipefd[1]);

    close(pipefd2[0]);
    close(pipefd2[1]);
 // execute cat
  execvp("cat", cat_args);
 }
}
**/

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char fixed_str_2[] = "gobison.org";
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of first pipe
//         close(fd2[0]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        wait(NULL); 
      
        char input_str_2[100];
        read(fd2[0], input_str_2, 100);
        int k = strlen(input_str_2);
        int index; 
        for (index=0; index < strlen(fixed_str_2); index++) 
            input_str_2[k++] = fixed_str_2[index];
        
        input_str_2[k] = '\0';
        printf("Second concatenated string %s\n", input_str_2);
        
      
  
        close(fd2[1]); // Close writing end of pipes 
        close(fd2[0]);
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
//         close(fd2[1]); 
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Concatenated string %s\n", concat_str);
      
        // Ask user for another string and overwrite old string.
        printf("Enter a string to concatenate:");
        scanf("%s", input_str); 
      
        write(fd2[1], input_str, strlen(input_str)+1);   
      
        // Close both reading ends 
        close(fd2[1]); 
        close(fd1[0]); 
        close(fd2[0]); 

  
        exit(0); 
    } 
} 