#include "../minishell.h"

void	init_envnode(t_env *env)
{
	if (!env)
		return ;
	env->name = NULL;
	env->value = NULL;
	env->prev = NULL;
	env->next = NULL;
}

t_env	*last_envlist(t_env *envlist)
{
	t_env	*current;

	current = NULL;
	current = envlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	envlist_append(t_env *node, t_env **list)
{
	t_env	*last_node;

	if (!node)
		return ;
	last_node = NULL;
	if (*list == NULL)
	{
		*list = node;
		(*list)->next = NULL;
		(*list)->prev = NULL;
	}
	else
	{
		last_node = last_envlist(*list);
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
	}
}

int	envlist_len(t_env *list)
{
	int		i;
	t_env	*current;

	if (list == NULL)
		return (0);
	current = list;
	i = 0;
	while (current->next)
	{
		current = current->next;
		i++;
	}
	i++;
	return (i);
}

/*list to matrix*/
char	**litoma(t_env *envlist)
{
	int		lenlist;
	t_env	*tmp_envlist;
	char	**env_matrix;
	int		len_name;
	int		len_value;
	int		i;
	int		n;
	int		m;
	//printf("litoma: procedo alla conversione in  matrice\n");
	tmp_envlist = envlist;
	lenlist = envlist_len(tmp_envlist);
	//printf("il numero degli elementi della matrice: %d\n", lenlist);
	env_matrix = (char **)malloc((lenlist + 1) * sizeof(char *));
	if (!env_matrix)
		return (NULL);
	i = 0;
	while (tmp_envlist)
	{
		len_name = ft_strlen(tmp_envlist->name);
		if (tmp_envlist->value)
			len_value = ft_strlen(tmp_envlist->value);
		else
			len_value = 0;
		env_matrix[i] = (char *)malloc((len_name + len_value + 2)
				* sizeof(char)); //uno per /0 e uno per uguale
		if (!env_matrix[i])
			return (NULL);
		//env_matrix[i][len_name + len_value + 2] = '\0';
		n = 0;
		m = 0;
		while (tmp_envlist->name[n])
		{
			env_matrix[i][m] = tmp_envlist->name[n];
			m++;
			n++;
		}
		env_matrix[i][m] = '=';
		m++;
		n = 0;
		if (tmp_envlist->value)
		{
			while (tmp_envlist->value[n])
			{
				env_matrix[i][m] = tmp_envlist->value[n];
				m++;
				n++;
			}

		}
		env_matrix[i][m] = '\0';
		
		//printf("ecco env_matrix[%d]: %s\n", i, env_matrix[i]);
		i++;
		tmp_envlist = tmp_envlist->next;
	}
	env_matrix[i] = NULL;
	//print_matrix_of_char(env_matrix);
	return (env_matrix);
}

