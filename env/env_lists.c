#include "../minishell.h"

void init_env(t_env *env)
{
	if (!env)
		return;
	env->name = NULL;
	env->value = NULL;
	env->prev = NULL;
	env->next = NULL;
}

t_env *last_envnode(t_env *envlist)
{
	t_env *current;
	
	current = envlist;
	while (current -> next != NULL)
		current = current -> next;
	return (current);
}

t_env *create_lenvnode(char *str)
{
	t_env *new;
	char **splittedstr;

	splittedstr = ft_split(str, '='); //TODO quando la frii in cao fai lo stradpo per i caori piu' sotto
	new = (t_env *)malloc(sizeof(t_env));
	init_env(new);
	new->name = splittedstr[0];
	new->value = splittedstr[1];
	return (new);
}

void listappend_envnode(t_env *node, t_env **list)
{
	t_env *last_node;

	if (*list == NULL)
	{
		//printf("Yes it is null now\n");
		*list = node;
		(*list)->next = NULL;
		(*list)->prev = NULL;
		//printf("hahaha: %s\n", (*list)->cmd);
	}
	else
	{
		//printf("it is no more null\n");
		last_node = last_envnode(*list);
		//printf("i found last node: %s\n", last_node->cmd);
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
		//printf("hehehe: %s\n", (*list)->cmd);
	}
}

int envlistlen(t_env *list)
{
	int i;
	t_env *current;
	
	if (list == NULL)
		return (0);
	current = list;
	i = 0;
	while (current -> next)
	{
		current = current->next;
		i++;
	}
	i++;
	return (i);
}
