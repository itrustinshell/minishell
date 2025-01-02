#include "../minishell.h"

t_env *access_envar(char *envar, t_env *envlist) //verifica se esiste la variabile
{
	t_env *tmp_envlist;

	if (!envlist)
	{
		printf("non esiste ancora alcuna variabile locale\n");
		return (NULL);
	}
	if (!envar)
	{
		printf("non hai indicato quale variabile vuoi controllare\n");
		return (NULL);
	}
	tmp_envlist = envlist;
	while (tmp_envlist)
	{
		if (strcmp(tmp_envlist->name, envar) == 0)
		{
			printf("SI! La variabile ESISTE\n");
			return (tmp_envlist);
		}
		tmp_envlist = tmp_envlist->next;
	}
	printf("Attenzione: la variabile NON esiste!\n");
	return (NULL);
}

t_env *nodedup(t_env *node)
{
	t_env *duplicated_node;

	if(!node)
		return (NULL);
	duplicated_node = (t_env *)malloc(sizeof(t_env));
	init_env(duplicated_node);
	duplicated_node->name = strdup(node->name);
	duplicated_node->value = strdup(node->value);
	printf("ecco il nodo duplicato:%s=%s\n",duplicated_node->name, duplicated_node->value);
	return (duplicated_node);
}

void ft_export(char *envar, t_env *local_envlist, t_env **global_envlist)
{
	t_env	*found_local_envar;
	t_env	*found_global_envar;

	t_env	*new_node;

//AAA ma deve funzionare cosi ? export ccc=345 cioè con questo input ? 
	
	found_local_envar = access_envar(envar, local_envlist);

	if (!found_local_envar)
		return;
	found_global_envar = access_envar(envar, *global_envlist);
	if(found_global_envar)
	{
		printf("la variabile già esiste nell'ambiente globale!");
		// free(found_global_envar->value);
		// found_global_envar->value = NULL;
		// found_global_envar->value = strdup(....aaa da implementare ...percè passi quello che c'è dopo =);
		return;
	}
	else
	{
		new_node = nodedup(found_local_envar);
		listappend_envnode(new_node, global_envlist);
	}

}