#include "declarations.h"
#include "myshell.c"
#include "alias.c"
#include "redirection_handler.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


int main(){

print_prompt_sign(prompt);
init_sh();
print_prompt_sign(prompt);

while(fgets(buffer,1024,stdin)!=NULL) {
read_command_line_ip(buffer);


}


}