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


void print_list(t_command *commandlist)
{

	t_command *current;
		int a;
		printf("i'm printing command list\n");
		current = commandlist;
		a = 0;
		while (current)
		{
			printf("cmd %d: %s\n", a, current->cmd);
			current = current->next;
			a++;
		}

}
