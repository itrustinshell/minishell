#include "minishell.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"



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
		printf("%s: ", cwdpath);
		getline(&inputstr, &len, stdin);
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
			single_cmd_ex(cmd); //al momento senza redirections
		}

		/***************************************final part******************************************************************************/
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
