#include "../minishell.h"

void test_stampa_args(t_command *commandlist)
{
	int i;

	while (commandlist -> next != NULL)
	{
		printf("this is the command:%s\n", commandlist -> cmd);
		i = 0;
		while (commandlist -> args[i])
		{
			printf("argument: %s\n", commandlist -> args[i]);
			i++;
		}
		commandlist = commandlist -> next;
	}
	printf("this last cmd: %s\n", commandlist-> cmd);
	i = 0;
	while (commandlist -> args[i])
	{
		printf("argument: %s\n", commandlist -> args[i]);
		i++;
	}
	printf ("it was already null\n");
}

