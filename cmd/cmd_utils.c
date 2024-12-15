#include "../minishell.h"

char *find_external_cmd(char *cmd)
{
	char *path;
	char **splitted_path;
	char *finalpath;
	int i;
	int access_result;

	path = getenv("PATH");
	splitted_path = ft_split(path, ':');

	i = 0;
	while (splitted_path[i])
	{
		finalpath = strjoin(splitted_path[i], "/");
		finalpath = strjoin(finalpath, cmd);
		access_result = access(finalpath, X_OK);
		if (access_result == 0)
			return (finalpath);
		//qui dovresti metter un else free l'attuale path creato con strjoin perchè strjoin malloca
		i++;
	}
	printf("command not found\n");
	return (NULL);
}

char *get_cmdpath(char *cmd) {
    if (strcmp(cmd, "echo") == 0)
        return "/home/ubuntu/mine/minishell/builtins/myecho";
    if (strcmp(cmd, "pwd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mypwd";
    if (strcmp(cmd, "cd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mycd";
    return find_external_cmd(cmd);
}