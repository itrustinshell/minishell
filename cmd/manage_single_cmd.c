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
	cmd->cmd = strdup(matrix[0]); 
	
	matrix_len = matrixlen(matrix);
	cmd->args = (char **)malloc((matrix_len + 1) * sizeof(char*));
	i = 0;
	while ( i < matrix_len)
	{
		cmd->args[i] = strdup(matrix[i]);	
		i++;
	}
	/*termino con NULL*/
	cmd->args[i] = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	//printf("created singe node for execution without pipe\n");
	return (cmd);
}


void	single_cmd_ex(t_command *cmd)
{
	pid_t		pid;
	int			status;

	//printf("sono appena entrato nella gestione del singolo comando\n");	

	cmd->path = get_cmdpath(cmd->cmd);

	pid = fork(); //forko altrimenti execve mi cancella la shell corrente
	if (pid == CHILD_PID0) //child
		execve(cmd->path, cmd->args, NULL);
	if (pid > CHILD_PID0) //parent
		waitpid(pid, &status, 0); // Attendi il processo figlio//status e'una variabile che ti consente di verificare come e' terminato il figlio ...il codice di terminazione
}

