#include "../minishell.h"

char **split_pathvariable(void)
{
    char **splitted_path;
	char *path;

    path = getenv("PATH");
    if (!path)
        return (NULL);
    splitted_path = ft_split(path, ':');
    if (!splitted_path)
        return (NULL);
    return (splitted_path);
}

char *build_cmd(char *root_path, char *cmd)
{
	char *full_cmd;
	char *path;

	if (!root_path || !cmd)
		return (NULL);
	path = ft_strjoin(root_path, "/");
	if (!path)
		return NULL;

	full_cmd = ft_strjoin(path, cmd);
	free(path);
	return (full_cmd);
}

/*
prima o poi farai un free dell'input.
Se Hai passato qualcosa di statico come "ls"
Puoi avee qualche probeminio nel freeare quello.
Ecco perchè ritorni uno strdup(stringa)
*/
char *get_cmdpath(char *cmd)
{
    char **splitted_path;
    int i;
	char *full_cmd;
	
	if (!cmd)
    	return (NULL);
	if (access(cmd, X_OK) == 0)
		return strdup(cmd);
	splitted_path = split_pathvariable();
	if (!splitted_path)
		return (NULL);
    i = 0;
    while (splitted_path[i])
    {	
		full_cmd = build_cmd(splitted_path[i], cmd);
		if (!full_cmd)
		{
       		free_matrix(splitted_path);
			return (NULL);
		}
        if (access(full_cmd, X_OK) == 0)
        {
            free_matrix(splitted_path);
            return (full_cmd);
        }
		free(full_cmd);
        i++;
    }
    free_matrix(splitted_path);
    return (NULL);
}

/* Crea un nuovo nodo t_cmd dalla matrice di comandi */
t_cmd *create_cmd(char **matrix)
{
	int			i;
	int			matrix_len;
	t_cmd	*cmd;

	if (!matrix || !matrix[0])
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = strdup(matrix[0]);
	if (!cmd->cmd)
	{
		free_cmd(cmd);
		return (NULL);
	}
	matrix_len = matrixlen(matrix);
	cmd->args = (char **)malloc((matrix_len + 1) * sizeof(char *));
	if (!cmd->args)
	{
		//free(cmd->cmd);
		free_cmd(cmd);
		return (NULL);
	}
	i = 0;
	while (i < matrix_len)
	{
		cmd->args[i] = strdup(matrix[i]);
		if (!cmd->args[i])
		{
			while (i > 0)
			{
				free(cmd->args[--i]);
			}
			//free(cmd->args);
			//free(cmd->cmd);
			free_cmd(cmd);
			return (NULL);
		}
		i++;
	}
	cmd->args[i] = NULL;
	cmd->next = NULL;
	return (cmd);
}
