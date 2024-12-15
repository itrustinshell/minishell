#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

char *get_command_path(char *cmd) {
    if (strcmp(cmd, "echo") == 0)
        return "/home/ubuntu/mine/minishell/builtins/myecho";
    if (strcmp(cmd, "pwd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mypwd";
    if (strcmp(cmd, "cd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mycd";
    return find_external_cmd(cmd);
}

int general_pipe_management(t_command *cmdlist, int cmdlist_len, int **pipesarray) {
    int i;
	int	j;
    pid_t pid;
    char *ext_cmd;

    // Ciclo per ogni comando
    i = 0;
	while (cmdlist)
	{ 
        pid = fork();
        if (pid == CHILD_PID) 
		{ 
            // Processo figlio
           // printf("Figlio %d: gestendo pipe %d\n", getpid(), i);
            
            if (i > 0) 
			{ 
                // Pipe precedente per input (leggi dal precedente)
                close(pipesarray[i - 1][WRITE_END]);  // Chiudi la scrittura della pipe precedente
                dup2(pipesarray[i - 1][READ_HERE_STDIN], STDIN_FILENO); // Duplica in ingresso
                close(pipesarray[i - 1][READ_END]);  // Chiudi il lato lettura
            }
            if (cmdlist->next) 
			{ 
                // Pipe corrente per output (scrivi nel successivo)
                close(pipesarray[i][READ_END]);  // Chiudi la lettura della pipe corrente
                dup2(pipesarray[i][WRITE_HERE_STDOUT], STDOUT_FILENO);  // Duplica in uscita
                close(pipesarray[i][WRITE_END]);  // Chiudi il lato scrittura
            } 
			else 
			{
                // Ultimo comando - chiudi entrambe le estremità della pipe
                close(pipesarray[i][READ_END]);
                close(pipesarray[i][WRITE_END]);
            }
            // Esegui il comando
            ext_cmd = get_command_path(cmdlist->cmd);
            execve(ext_cmd, cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);
        } 
		else if (pid > 0) 
		{ 
            // Processo padre
            cmdlist = cmdlist->next;
            i++;
        } 
		else 
		{
            perror("Errore fork");
            exit(1);
        }
    }
    // Ora il padre chiude tutte le pipe
    j = 0;
	while (j < (cmdlist_len - 1)) 
	{
        close(pipesarray[j][READ_END]);
        close(pipesarray[j][WRITE_END]);
		j++;
    }
    // Il padre attende la terminazione di tutti i figli
	i = 0;
    while (i < cmdlist_len) 
	{
		wait(NULL);
		i++;
    	//printf("Figlio terminato (PID: %d)\n", pid);
    }
    return 0;
}
