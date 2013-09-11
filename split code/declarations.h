#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


#define MAX_LENGTH 256


	
	 

void read_profile_variable(char *token, char *var_argv);
//void  ALARMhandler(int sig);
void print_prompt_sign(char *prompt);
void init_sh();
void exit_handler(char *buffer);
void change_directory(char *LHS, char *dest);
void alias_handler(char **argv);
void  parse(char *line, char **argv);
void handle_others(char **argv);
void redirection_handler(char **argv);
void count_pipes(char **argv);
void read_command_line_ip(char *buffer);
void execute(char **argv);


