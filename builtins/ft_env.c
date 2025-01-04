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

t_env *copy_envp(char **envp)
{
	t_env	*envlist;
	t_env	*envnode;
	int		i;
	
	envnode = NULL;
	envlist = NULL;
	//printf("il numero di variabili individuate è: %d\n", matrixlen(envp));
	i = 0;
	while(envp[i])
	{
		envnode = create_lenvnode(envp[i]);
		if (!envnode)
		{
			ft_freelist(envlist);
			return (NULL);
		}
		envlist_append(envnode, &envlist);
		i++;
	}
	return (envlist);
}