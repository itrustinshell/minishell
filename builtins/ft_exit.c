#include "../minishell.h"

void	ft_exit(int n_args, char **args)
{
	int exit_code;

	exit_code = 0;
	if (n_args > 2)
		printf("minishell: exit: too many arguments\n");
	if (n_args == 2)
		exit_code = atoi(args[1]);
	exit(exit_code);
}