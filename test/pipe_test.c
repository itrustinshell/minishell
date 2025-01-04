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

void printlist(t_command *cmdlist) //solo per testing
{
	t_command	*tmp_cmdlist;
	t_redir		*tmp_redirlist;
	int			m;

	printf("\nDEBUG: STO STAMPANDO LA LISTA DI COMANDI CON ARGOMENTI E REDIRECTION\n");
	m = 0;
	tmp_cmdlist = cmdlist;
	while (tmp_cmdlist)
	{
		printf("ecco il comando: %s\n", tmp_cmdlist->cmd);
		if (tmp_cmdlist->args)
		{
			printf("ecco gli argomenti di %s:\n", tmp_cmdlist->cmd);
			m = -1;
			while(tmp_cmdlist->args[++m])
				printf("args: %s\n", tmp_cmdlist->args[m]);
		}
		if (tmp_cmdlist->redirlist)
		{
			printf("ecco le redirections di %s:\n", tmp_cmdlist->cmd);
			tmp_redirlist = tmp_cmdlist->redirlist;
			while (tmp_redirlist)
			{
				printf("redir: %s\n", tmp_redirlist->outredir_file);
				tmp_redirlist = tmp_redirlist->next;
			}
		}
		else
			printf("non ci sno argomenti\n");
		tmp_cmdlist = tmp_cmdlist->next;
	}
	printf("DEBUG: HO TERMINATO DI STAMPARE LISTA, ARGOMENTI E RELATIVE REDIRECTIONS\n\n");
}