nclude <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>

void  ALARMhandler(int sig)
{
    char prompt[30]={'\0'};
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
    printf("Success");
  }
  else
    perror("ch_dir error");
	}

}

void alias_handler(char **argv){
char alias[100];
FILE *file = fopen("./.aliases", "a+" );
            if(file != NULL){
			strcat(alias,argv[1]);
			strcat(alias,argv[2]);
			strcat(alias,argv[3]);
            strcat(alias,"\n");
                fputs(alias,file);
                fclose(file);
            }
}

void  execute(char **argv)
{
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

void  parse(char *line, char **argv)
{
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';
        *argv++ = line;
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++;
    }
    *argv = '\0';
}


void handle_others(char **argv){

printf("arg is %s", *argv);

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
					argv[0]=command;
					}
					}
			
	
}
    execute(argv);

}

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
     fd = open(file, O_APPEND);
     if(fd<0)
     {
       fd = creat(file, 0666);
       if(fd<0)
       {
         perror("create file failed");
         exit(-1);
       }
     }
     dup(fd);
     execute(argv);
     close(fd);
}



int main(){
    char cmdline[1024];
	
	char prompt[30]={'\0'};
	char buffer[1024];
	int count;
	char *argv[64];
	
	
	print_prompt_sign(prompt);
	 init_sh();
print_prompt_sign(prompt);

	sigset_t sigs;

        sigemptyset(&sigs);
        sigaddset(&sigs, SIGINT);
        sigprocmask(SIG_BLOCK, &sigs, 0);
	while(fgets(buffer,1024,stdin)!=NULL) {
    strcpy(cmdline,buffer);
    if (strcmp(buffer ,"\n")!=0){
	buffer[strlen(buffer)-1] = '\0';
    parse(buffer, argv);

	 
 	char *tkn = strtok(buffer, " \t\n");
	
	exit_handler(tkn);
	change_directory(tkn,argv[1]);
	
	
	
	int i=0;
        int redirection = 0;
	while(argv[i]!=NULL){
		if(strcmp(argv[i],">")==0){
			redirection_handler(argv);
            redirection = 1;
			break;
		}
		i++;
	}
	
    printf("%sToken",tkn);
        if(redirection == 0){
            if(strcmp(tkn, "alias")==0){
                alias_handler(argv);
            }else{
                handle_others(argv);}}
    print_prompt_sign(prompt);
    }
    }
}


