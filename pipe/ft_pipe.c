#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>



int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) {
    int i = 0;
    pid_t pid;


printf("vediamo sta lista;\n");

while (cmdlist)
{
    printf("%s\n", cmdlist->cmd);
    cmdlist = cmdlist->next;
}


if (cmdlist)
    printf("la lista esiste\n");


    while (cmdlist) 
	{
        pid = fork();
        if (pid == CHILD_PID) 
		{ 
			// Processo figlio
            if (i > 0) 
			{ 
				// Pipe precedente per input
                close(pipematrix[i - 1][WRITE_END]);
                dup2(pipematrix[i - 1][READ_HERE_STDIN], STDIN_FILENO);
                close(pipematrix[i - 1][READ_END]);
            }
            if (cmdlist->next) 
			{ 
				// Pipe corrente per output
                close(pipematrix[i][READ_END]);
                dup2(pipematrix[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
                close(pipematrix[i][WRITE_END]);
            }
            cmdlist->path = get_cmdpath(cmdlist->cmd);
            execve(cmdlist->path, cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);
        } 
		else if (pid > 0) 
		{ 
			// Processo padre man mano che si avanza chiude le pipe precedentamente aperte.
            if (i > 0) 
			{
                close(pipematrix[i - 1][READ_END]);
                close(pipematrix[i - 1][WRITE_END]);
            }
            cmdlist = cmdlist->next;
            i++;
        } 
		else 
		{
            perror("Errore fork");
            exit(1);
        }
    }
    while ((pid = wait(NULL)) > 0) 
	{
      //  printf("Figlio terminato (PID: %d)\n", pid);
    }
    return 0;
}
