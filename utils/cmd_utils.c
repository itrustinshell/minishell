#include "../minishell.h"

char	**split_pathvariable(void)
{
	char	**splitted_path;
	char	*path;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	splitted_path = ft_split(path, ':');
	if (!splitted_path)
		return (NULL);
	return (splitted_path);
}

char	*build_cmd(char *root_path, char *cmd)
{
	char	*full_cmd;
	char	*path;

	if (!root_path || !cmd)
		return (NULL);
	path = ft_strjoin(root_path, "/");
	if (!path)
		return (NULL);
	full_cmd = ft_strjoin(path, cmd);
	free(path);
	return (full_cmd);
}

char	*get_cmdpath(char *cmd)
{
	char	**splitted_path;
	int		i;
	char	*full_cmd;

	if (!cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));
	splitted_path = split_pathvariable();
	if (!splitted_path)
		return (NULL);
	i = -1;
	while (splitted_path[++i])
	{
		full_cmd = build_cmd(splitted_path[i], cmd);
		if (!full_cmd)
		{
			free_matrix(splitted_path);
			return (NULL);
		}
		if (access(full_cmd, X_OK) == 0)
			return (free_matrix(splitted_path), full_cmd);
		free(full_cmd);
	}
	return (free_matrix(splitted_path), NULL);
}
