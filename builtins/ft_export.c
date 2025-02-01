#include "../minishell.h"

int	ends_with_equal_sign(char *str)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (str[i])
		i++;
	i--;
	if (str[i] == '=')
		ret = 1;
	return (ret);
}

int	there_is_equal_before_end(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	exportcheck(char **matrix)
{
	if (matrixlen(matrix) > 3)
	{
		printf("ci sono piu di 3 pezzi con l'export\n");
		return (0);
	}
	if (!matrix[1])
	{
		printf("non c'è il pezzo con l'uguale\n");
		return (0);
	}
	if (!ends_with_equal_sign(matrix[1]))
	{
		if (!there_is_equal_before_end(matrix[1]))
			return (0);
		else if (matrixlen(matrix) >= 3)
		{
			printf("troppi argomenti\n");
			return (0);
		}
		else
		{
			printf("ok qui puoi lavorare per estrarre la variabile\n");
			return (1);
		}
	}
	else	{
		if (matrixlen(matrix) > 3)
		{
			printf("AAA: il simbolo di uguale c'è ma ci sono troppi argomenti\n");
			return (0);
		}
		else
		{
			printf("si la prima parte termina con il simbolo uguale\n");
			return (2);
		}
	}
	return 0;
}

void addto_globalenv(t_env **local_envlist, char *str)
{
	t_env *local_envnode;

	local_envnode = create_lenvnode(str);
	if (local_envnode)
		printf("il nodo esiste. Il name è %s, mentre il valore è %s\n", local_envnode->name, local_envnode->value);
	envlist_append(local_envnode, local_envlist);
	if (local_envlist)
		printf("finalmente la lista estiste\n");
	else
		printf("attenzione ho aggiunto un nodo ma c'è qualcosa che non va: la lista non esista\n");	
}

t_env *access_envar(char *namevar, t_env *list) //verifica se esiste la variabile
{
	printf("sono alla ricercaquesta variabile: %s\n", namevar);
	t_env	*tmp_list;

	if (!list)
	{
		printf("non esiste ancora alcuna variabile globlae\n");
		return (NULL);
	}
	if (!namevar)
	{
		printf("non hai indicato quale variabile vuoi controllare\n");
		return (NULL);
	}
	tmp_list = list;
	while (tmp_list)
	{
		printf("ecco tmp_list->name: %s\n", tmp_list->name);
		printf("namevar: %s\n", namevar);
		if (strcmp(tmp_list->name, namevar) == 0)
		{
			printf("SI! La variabile ESISTE\n");
			return (tmp_list);
		}
		tmp_list = tmp_list->next;
	}
	printf("Attenzione: la variabile NON esiste!\n");
	return (NULL);
}

t_env	*create_lenvnode(char *str)
{
	t_env	*new;
	char	**splittedstr;
	//printf("i'm creating a newnode\n");
	new = NULL;
	splittedstr = NULL;
	splittedstr = ft_split(str, '=');
	if (!splittedstr)
		return (NULL);
	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
	{
		return (NULL);
	}
	init_envnode(new);
	new->name = strdup(splittedstr[0]);
	if (splittedstr[1] != NULL)	
		new->value = strdup(splittedstr[1]);
	return (new);
}

void	update_envar_value(t_env *found_global_envar, char *envar_value)
{
	if (strcmp(found_global_envar->value, envar_value) == 0)
	{
		printf("non c'é bisogno di aggiornare\n");
		return ;
	}
	printf("bisogna aggiornare\n");
	free(found_global_envar->value);
	found_global_envar->value = strdup(envar_value);
	//free(envar_value);
	
	return;
		
}

void	ft_export(char *envar_name, t_env **global_envlist)
{
	t_env	*found_global_envar;
	t_env	*new_node;

	new_node = create_lenvnode(envar_name);
	if (new_node)
		printf("il nodo esiste. Il name è %s, mentre il valore è %s\n", new_node->name, new_node->value);
	found_global_envar = access_envar(new_node->name, *global_envlist);
	if (found_global_envar)
	{
		update_envar_value(found_global_envar, new_node->value);
		printf("la variabile già esiste nell'ambiente globale!");
		return ;
	}
	else
		envlist_append(new_node, global_envlist);
}
