#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int matrixlen(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		i++;
	return (i);
}

t_command *create_cmd(char **matrix)
{
	int			i;
	int			cmd_index;
	int			matrix_len;
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	cmd -> cmd = strdup(matrix[0]); 
	
	matrix_len = matrixlen(matrix);
	cmd -> args = (char **)malloc((matrix_len + 1) * sizeof(char*));
	i = 0;
	while ( i < matrix_len)
	{
		cmd -> args[i] = strdup(matrix[i]);	
		i++;
	}
	/*termino con NULL*/
	cmd->args[i] = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	printf("created singe node for execution without pipe\n");
	return (cmd);
}

void	execute_cmd(t_command *cmd)
{
	pid_t		pid;
	printf("sono appena entrato nella gestione del singolo comando\n");	
		if (strcmp(cmd->cmd, "echo") == 0)
			cmd->path = "/home/ubuntu/mine/minishell/builtins/myecho";
		else if (strcmp(cmd->cmd, "pwd") == 0)
			cmd->path = "/home/ubuntu/mine/minishell/builtins/mypwd";
		else if (strcmp(cmd->cmd, "cd") == 0)
			cmd->path = "/home/ubuntu/mine/minishell/builtins/mycd";
		else 
			cmd->path = find_external_cmd(cmd->cmd);

	pid = fork(); //forko altrimenti execve mi cancella la shell corrente
	if (pid == CHILD_PID0) //child
	{
		int pidpid = getpid();
		printf("sono nel child dopo il fork il nel child è: %d\n", pid);
		printf("sono nel child (pidpid: %d) del single command management e sto per eseguire execve\n", pidpid);
		
		execve(cmd->path, cmd->args, NULL);
		
	}
	else if (pid > CHILD_PID0) //parent
	{
		int status;
        pid_t child_pid = waitpid(pid, &status, 0); // Attendi il processo figlio

		printf("single cmd: father is waiting\n");
		printf("sono nel father, dopo il fork il pid nel father è: %d\n", pid);
		if (WIFEXITED(status)) 
		{
			printf("Il figlio %d è terminato con codice %d\n", child_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status))
		{
			printf("Il figlio %d è terminato a causa del segnale %d\n", child_pid, WTERMSIG(status));
		}
		printf("maibe is not waiting\n");
	}
}

