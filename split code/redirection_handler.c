#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>

          

void redirection_handler(char **argv){

    char *file;
    int i=0;
    while(*argv!=NULL){
        if(strcmp(argv[i],">")==0){
            file = argv[i+1];
            break;
        }
    	i++;
    }


	
    int fd;
    close(1);
    fd = open(file,O_APPEND);
    if(fd<0)
    {
        fd = creat(file, 0666);
        if(fd<0)
        {
            perror("create file failed");
            exit(-1);
        }
    }
	
	 
		 pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {
        if (execvp(*argv, argv) < 0) {
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else {
        while (wait(&status) != pid);


    }
	
    
}