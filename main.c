#include "minishell.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/wait.h>
#include <sys/types.h>

int check_redirection_symbol(char **tokenmatrix)
{
	int	i;

	i = 0;
	while (tokenmatrix[i])
	{
		if (tokenmatrix[i][0] != OUTPUT_REDIRECTION)
			i++;
		else
		{
			//printf("there is a pipe\n");
			return (THERE_IS_A_REDIRECTION);

		}
	}
	return (THERE_IS_NOT_A_REDIRECTION);
}

void cmd_before_single_outredirection(char **matrix)
{
	t_command	*cmd;
	int			i;
	int			fd;

	cmd = (t_command *)malloc(sizeof(t_command));
	
	cmd->cmd = strdup(matrix[0]);
	cmd->path = get_cmdpath(cmd->cmd);
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
		i++;
	cmd->args = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
	{
		cmd->args[i] = strdup(matrix[i]);
		i++;
	}
	cmd->args[i] = NULL;
	i++;
	fd = open(matrix[i], O_WRONLY | O_CREAT | O_TRUNC, 0466); //TODO aprire il file dato in path assoluto e relativo
	
	int pid = fork();

	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execve(cmd->path, cmd->args, NULL);
	}
	else
	{
		close(fd);
		wait(NULL);
	}		
}


int main(int argc, char **argv)
{
	char *inputstr;
	char *cwdpath;
	size_t len;
	char **matrix;
	int i;
	t_command *cmd;
	t_command	*cmdlist;
	int			cmdlist_len;
	int			**pipematrix;
	char *buffer = NULL;

	
	while (1)
	{	
		len = 0;
		inputstr = NULL;
	
		cwdpath = getcwd(NULL, 0);
		printf("%s💪💪💪: ", cwdpath);
		getline(&inputstr, &len, stdin); //free di inputstr fatto!
		matrix = tokenizer(inputstr);
		//print_matrix_of_char(matrix);

		if (strcmp(matrix[0], "pwd") == 0)
		{
			buffer = getcwd(NULL, 0);
			printf("%s\n", buffer);
			//free(buffer);
			return (0);
		}
		if (check_pipe_symbol(matrix) == THERE_IS_A_PIPE)
		{
			cmdlist = commandlist_for_pipe(matrix); //se c'è almeno una pipe, viene costruita una lista di comandi
			cmdlist_len = listlen(cmdlist);
			pipematrix = pipematrix_malloc(cmdlist_len);
			pipex(cmdlist, cmdlist_len, pipematrix);
		}
		else
		{
			cmd = create_cmd(matrix);
			//printf("AAAAAAA IO VENGO DAL MAIN\n");
			single_cmd_ex(cmd); //al momento senza redirections
		}
		/*else if (check_redirection_symbol(matrix) == THERE_IS_A_REDIRECTION)
		{
			printf("there is a redirection\n");
			cmd_before_single_outredirection(matrix);
		}
	*/

		/*********************************final part*******************************************/
		i = 0;
		while (matrix[i])
		{
			free(matrix[i]);
			matrix[i] = NULL;
			i++;
		}
		free(matrix);
		matrix = NULL;
		free(inputstr);
	}
	return (0);
}
