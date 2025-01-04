#include "../minishell.h"

void init_cmd(t_command *cmd)
{
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->path = NULL;
    cmd->next = NULL;
    cmd->redirlist = NULL;
}

char *get_cmdpath(char *cmd)
{
	char *path;
	char **splitted_path;
	char *finalpath;
	int i;
	int access_result;

	if (cmd)
//		printf("il comando esiste: %s\n", cmd);
	path = getenv("PATH");
//	printf("ecco la variabile PATH: %s\n", path);
	splitted_path = ft_split(path, ':');

	i = 0;
	while (splitted_path[i])
	{
		finalpath = strjoin(splitted_path[i], "/");
		finalpath = strjoin(finalpath, cmd);
//		printf("verifico l'esistenza di: %s\n", finalpath);
		access_result = access(finalpath, X_OK);
		if (access_result == 0)
			return (finalpath);
		//qui dovresti metter un else free l'attuale path creato con strjoin perchè strjoin malloca
		i++;
	}
	printf("NON HO TROVATO IL COMANDO!!!\n");
	return (NULL);
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

