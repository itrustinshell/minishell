#include "../minishell.h"

int printenvlist(t_env *lenvlist)
{
	t_env *tmp_envlist;

	if (!lenvlist)
	{
		//printf("non posso stampare la lista perchè non esiste\n");
		return (0);
	}
	else
		//printf("ok la lista esiste e ora la stampo\n");
	tmp_envlist = lenvlist;
	while (tmp_envlist)
	{
		printf("%s=%s\n",tmp_envlist->name, tmp_envlist->value);
		tmp_envlist = tmp_envlist->next;
	}
	return (1);
}

