#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void alias_handler(char **argv){
    
    int i=0;
    int count=0;
    printf("\n");
    while (argv[i]!=NULL){
        if((strcmp(argv[i], " ")!=0) || (strcmp(argv[i], "\n") != 0) || (strcmp(argv[i],"\0")!=0)){
            count++;
        }
        i++;
        
    }
    if(count <= 3){
        printf("Not enough arguments for alias. Cannot set the alias\n");
        return;
        
    }
    
    char alias[100];
    FILE *file = fopen("./.aliases", "a+" );
    if(file != NULL){
      
        fputs(argv[1],file);
        fputs(argv[2],file);
        fputs(argv[3],file);
        fputs("\n",file);
        fclose(file);
    }
}