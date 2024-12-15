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
/**/
int general_pipe_management(t_command *cmdlist, int cmdlist_len, int **pipesarray) {
    int i = 0;
    pid_t pid;
    char *ext_cmd;

    while (cmdlist) 
	{
        pid = fork();
        if (pid == CHILD_PID) 
		{ 
			// Processo figlio
            if (i > 0) 
			{ 
				// Pipe precedente per input
                close(pipesarray[i - 1][WRITE_END]);
                dup2(pipesarray[i - 1][READ_HERE_STDIN], STDIN_FILENO);
                close(pipesarray[i - 1][READ_END]);
            }
            if (cmdlist->next) 
			{ 
				// Pipe corrente per output
                close(pipesarray[i][READ_END]);
                dup2(pipesarray[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
                close(pipesarray[i][WRITE_END]);
            }
            ext_cmd = get_command_path(cmdlist->cmd);
            execve(ext_cmd, cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);
        } 
		else if (pid > 0) 
		{ 
			// Processo padre
            if (i > 0) 
			{
                close(pipesarray[i - 1][READ_END]);
                close(pipesarray[i - 1][WRITE_END]);
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

