/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   litoma.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:36:08 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:52:06 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

/*+2 becouse i consider the symbols '=' and '\0'*/
int	get_envmatrixelement_len(t_env *tmp_envlist)
{
	int	len;

	len = ft_strlen(tmp_envlist->name) + 2;
	if (tmp_envlist->value)
		len += ft_strlen(tmp_envlist->value);
	return (len);
}

void	fill_matrixelement(char *matrixelement, t_env *envnode)
{
	int	i;
	int	j;

	i = 0;
	while (envnode->name[i])
	{
		matrixelement[i] = envnode->name[i];
		i++;
	}
	matrixelement[i] = '=';
	i++;
	j = 0;
	if (envnode->value)
	{
		while (envnode->value[j])
		{
			matrixelement[i] = envnode->value[j];
			i++;
			j++;
		}
	}
	matrixelement[i] = '\0';
}

char	**litoma(t_env *envlist)
{
	char	**env_matrix;
	t_env	*tmp_envlist;
	int		i;
	int		lenlist;
	int		len_matrixelement;

	tmp_envlist = envlist;
	lenlist = envlist_len(tmp_envlist);
	env_matrix = matrixalloc(lenlist);
	if (!env_matrix)
		return (NULL);
	i = 0;
	while (tmp_envlist)
	{
		len_matrixelement = get_envmatrixelement_len(tmp_envlist);
		env_matrix[i] = malloc(len_matrixelement * sizeof(char));
		if (!env_matrix[i])
			return (NULL);
		fill_matrixelement(env_matrix[i], tmp_envlist);
		i++;
		tmp_envlist = tmp_envlist->next;
	}
	env_matrix[i] = NULL;
	return (env_matrix);
}
