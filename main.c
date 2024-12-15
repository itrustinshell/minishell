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
	int			**pipesarray;
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
			cmdlist_len = number_of_nodes_in_a_list(cmdlist);
			pipesarray = create_pipesarray(cmdlist_len);
			general_pipe_management(cmdlist, cmdlist_len, pipesarray);
		}
		else
		{
			cmd = create_cmd(matrix);
			execute_cmd(cmd); //al momento senza redirections
		}
		printf("main: libero la matrix\n");	
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
