

#include "../minishell.h"

//TODO: gestire gli exit code...il momento in cui agirnare o stampare con echo, e il ritorno di tutti gli altri.

/*
	DESCRIPTION
		simula il comportamento di echo.
*/
int	ft_echo(int argc, char **argv, int *exit_code)
{
	int	i;

	if (argv[1] && strcmp(argv[1], "-n") == 0)
	{
		i = 2;
		while (i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("%s", argv[i]);
	}
	else if (argc == 2 && strcmp(argv[1], "$?") == 0)
		printf("Last process exit-code: %d\n", *exit_code);
	else
	{
		if (argc == 1)
		{
			printf("\n");
			return (1);
		}
		i = 1;
		while (i < argc - 1)
		{
			if (argv[i][0] == '$' && argv[i][1] == '?')
				printf("%d ", *exit_code);
			else
				printf("%s ", argv[i]);
			i++;
		}
		if (argv[i][0] == '$' && argv[i][1] == '?')
				printf("%d\n", *exit_code);
		else
			printf("%s\n", argv[i]);
	}
	return (1);
}
