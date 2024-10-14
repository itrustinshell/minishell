#include "../minishell.h"

char *find_external_cmd(char *cmd)
{
	char *path;
	char **splitted_path;
	char *finalpath;
	int i;
	int access_result;

	path = getenv("PATH");
	
	printf("i m in path: i print the entire path: %s\n", path);
	splitted_path = ft_split(path, ':');

   print_matrix_of_char(splitted_path);	
	i = 0;
	while (splitted_path[i])
	{
		finalpath = strjoin(splitted_path[i], "/");
		finalpath = strjoin(finalpath, cmd);
		access_result = access(finalpath, X_OK);
		if (access_result == 0)
		{
			printf("command found\n");
			printf("%s\n",finalpath);
			return (finalpath);
		}
		i++;
	}
	printf("command not found\n");
	return (NULL);
}
