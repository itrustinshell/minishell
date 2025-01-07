

#include "../minishell.h"

//TODO: gestire gli exit code...il momento in cui agirnare o stampare con echo, e il ritorno di tutti gli altri.

int	ft_echo(int argc, char **argv, int *exit_code)
{
	int	i;

	(void)exit_code;
	if (strcmp(argv[1], "-n") == 0)
	{
		i = 2;
		while (i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("%s", argv[i]);
	}
	else
	{
		i = 1;
		while (i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("%s\n", argv[i]);
	}
	return (1);
}
