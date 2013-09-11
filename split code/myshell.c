#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>



	char prompt[30]={'\0'};
	char buffer[1024];
	int count;
	char *argv[64];

int i=0;
            int redirection = 0;
            int piping =0;

void  ALARMhandler(int sig)
{

	char *a;
    signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
    printf("Do you want to exit the shell? \n");
    scanf("%s", a);
    if((strcmp(a, "y") == 0) || (strcmp(a, "Y") == 0) || (strcmp(a, "yes") == 0)
       || (strcmp(a, "YES") == 0)){
        exit(0);
    }

    signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}


void read_profile_variable(char *token, char *var_argv){
    char *result = token;
    const char *search = "=";
    static const char fileargv[] = ".myshell_profile";

    FILE *file = fopen ( fileargv, "r" );
    if ( file != NULL )
    {
        char line[100];

        int i =1;
        while ( fgets ( line, sizeof(line), file ) != NULL )
        {
            result = strtok(line,"=");
            while(result != NULL){
                if(strcmp(result,var_argv)==0){
                    result = strtok(NULL,"=");
                    strcpy(token,result);
                    fclose(file);
                    return;

                }
                result=strtok(NULL,"=");
            }
        }
    }
    fclose ( file );
}

void print_prompt_sign(char *prompt){
    if(strlen(prompt)==0){
        read_profile_variable(prompt,"SIGN");
    }
    printf("%s", prompt);
}

void init_sh()
{
    char home_dir[30];
    read_profile_variable(home_dir,"HOME");
    home_dir[strlen(home_dir)-1]='\0';
    printf("%s",home_dir);
    if(chdir(home_dir)==0)
    {
        setenv("HOME", home_dir, 1);
    }
    else
        perror("home directory initialization error");
}

void exit_handler(char *buffer){
	if(strcmp(buffer, "exit")==0){
		fflush(stdout);
		exit(0);
	}

}

void change_directory(char *LHS, char *dest){

	if(strcmp(LHS, "cd")==0){

        if(dest==NULL)
        {
            dest = getenv("HOME");
        }
        if(chdir(dest)==0)
        {
            //printf("Success");
        }
        else
            perror("ch_dir error");
	}

}






int is_spl(char *check, char *spl)
{
	while (*spl != '\0')
	{
		if (*check == *spl)
			return 1;
		spl++;
	}
	return 0;
}

void  parse(char *line, char **argv)
{
	/* char *spl = ">|";
	char *before, *after;
	int no_spl = 1,no_skip = 1;
    while (*line != '\0') {
    	no_skip = 1;
        while (*line == ' ' || *line == '\t' || *line == '\n')
        	if (*line == ' ')
        	{
        		after = line;
        		after++;
        		before = line;
        		before--;

        		if (is_spl(after,spl))
        		{
        			no_spl = 0;
        			*line++ = '\0';
        		}
        		else if (is_spl(before,spl) || no_spl)
        		{
        			*line++ = '\0';
        		}
        		else
        		{
        			line++;
        			no_skip = 0;
        		}

        	}
        	else
        		*line++ = '\0';
        if(no_skip)
        	*argv++ = line;
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++;
    }
    *argv = '\0'; */
	
	 while(*line !='\0')
  {
    while(*line == ' ' || *line == '\t' ||
        *line == '\n')
      *line++ = '\0';
    *argv++ = line;
    while(*line != '\0' && *line != ' ' &&
        *line != '\t' && *line != '\n')
      line++;
  }
  *argv = '\0';
}




void count_pipes(char **argv){
    
    
    struct command{
        char main_command[64];
        char *command_with_arguements[64];
    
    };
    
    struct command to_run[64];    
    int i=0;
    char commands[64][64];
    int k = -1;
    int counter = 0;
    int command_inc = 0;
    while(argv[i] !=NULL) {
        if(strcmp(argv[i],"|")==0){
            k=k+1;
            counter = 0;
            strcpy(to_run[command_inc].main_command,argv[k]);

            to_run[command_inc].command_with_arguements[counter]=argv[k];
            
            k++;
            for (; k<i; k++) {
                counter ++;
                to_run[command_inc].command_with_arguements[counter]=argv[k];
            }
            command_inc= command_inc+1;
        }
        
        i++;
    }
    
    k++;
    counter = 0;
    strcpy(to_run[command_inc].main_command,argv[k]);
    to_run[command_inc].command_with_arguements[counter]=argv[k];
    k++;
    for (; k<i; k++) {
        ++counter;
        to_run[command_inc].command_with_arguements[counter]=argv[k];
    }

/* printf(" arg in read %s \n", to_run[0].main_command);
printf(" arg in read %s",  to_run[0].command_with_arguements[0]);
printf(" arg in read %s", to_run[1].main_command);
printf(" arg in read %s",  to_run[1].command_with_arguements[0]);
printf(" arg in read %s",  to_run[1].command_with_arguements[1]); */

 int fd[2];
      pipe(&fd[0]);
      if(fork()!=0)
      {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        execvp(to_run[0].main_command, to_run[0].command_with_arguements);
        close(fd[1]);
      }
      else
      {
        close(fd[1]);
        close(0);
        dup(fd[0]);
         execvp(to_run[1].main_command, to_run[1].command_with_arguements);
        close(fd[0]);
      }
    
}



 
 
 
void read_command_line_ip(char *buffer){

            if (strcmp(buffer ,"\n")!=0){
            buffer[strlen(buffer)-1] = '\0';
            parse(buffer, argv);


            char *tkn = strtok(buffer, " \t\n");

            exit_handler(tkn);
            change_directory(tkn,argv[1]);



            while(argv[i]!=NULL){
                if(strcmp(argv[i],">")==0){
                    redirection_handler(argv);
                    redirection = 1;
                    break;
                }
                 else if(strcmp(argv[i],"|")==0){
                    count_pipes(argv);

                    piping=1;
                    break;
                }else  if(strcmp(tkn, "alias")==0){
                    alias_handler(argv);
                    print_prompt_sign(prompt);
                    return;
					}
                i++;
            }



    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {
        FILE *file = fopen("./.aliases", "r");
        if(file!=NULL) {
            char line[128];
            while ( fgets ( line, sizeof(line), file ) != NULL ){
                char *search = "=";
                char *result = NULL;
                char *command = NULL;
                result = strtok(line,"=");
                if(strcmp(argv[0],result)==0){
                    command= strtok(NULL,"=");
                    command[strlen(command)-1]='\0';
                    strcpy(argv[0],command);
                    fclose(file);
                }
            }
        }
        if (execvp(*argv, argv) < 0) {
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else {
        while (wait(&status) != pid);


    }
            print_prompt_sign(prompt);
        }

		}
