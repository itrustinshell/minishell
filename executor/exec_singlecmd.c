#include "../minishell.h"


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
	//cmd->prev = NULL;
	cmd->next = NULL;
	//printf("created singe node for execution without pipe\n");
	return (cmd);
}
