#include "../minishell.h"

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

