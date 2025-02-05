/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_lists.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:37:09 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:52:13 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (!last_node)
			return ;
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
	}
}

t_env	*create_lenvnode(char *str)
{
    t_env	*new;
    char	**splittedstr;

    splittedstr = ft_split(str, '=');
    if (!splittedstr)
        return (NULL);

    new = (t_env *)malloc(sizeof(t_env));
    if (!new)
        return (free_matrix(splittedstr), NULL);

    init_envnode(new);
    new->name = strdup(splittedstr[0]);
    if (splittedstr[1] != NULL)
        new->value = strdup(splittedstr[1]);

    free_matrix(splittedstr);  // ✅ Libera la memoria di splittedstr
    return (new);
}
