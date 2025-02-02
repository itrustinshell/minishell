/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:44:25 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:51:11 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	addto_globalenv(t_env **local_envlist, char *str)
{
	t_env	*local_envnode;

	local_envnode = create_lenvnode(str);
	envlist_append(local_envnode, local_envlist);
}

t_env	*access_envar(char *namevar, t_env *list)
{
	t_env	*tmp_list;

	if (!list)
		return (NULL);
	if (!namevar)
		return (NULL);
	tmp_list = list;
	while (tmp_list)
	{
		if (strcmp(tmp_list->name, namevar) == 0)
			return (tmp_list);
		tmp_list = tmp_list->next;
	}
	return (NULL);
}

t_env	*create_lenvnode(char *str)
{
	t_env	*new;
	char	**splittedstr;

	new = NULL;
	splittedstr = NULL;
	splittedstr = ft_split(str, '=');
	if (!splittedstr)
		return (NULL);
	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	init_envnode(new);
	new->name = strdup(splittedstr[0]);
	if (splittedstr[1] != NULL)
		new->value = strdup(splittedstr[1]);
	return (new);
}

//free(envar_value);
void	update_envar_value(t_env *found_global_envar, char *envar_value)
{
	if (strcmp(found_global_envar->value, envar_value) == 0)
		return ;
	free(found_global_envar->value);
	found_global_envar->value = strdup(envar_value);
	return ;
}

void	ft_export(char *envar_name, t_env **global_envlist)
{
	t_env	*found_global_envar;
	t_env	*new_node;

	new_node = create_lenvnode(envar_name);
	found_global_envar = access_envar(new_node->name, *global_envlist);
	if (found_global_envar)
	{
		update_envar_value(found_global_envar, new_node->value);
		return ;
	}
	else
		envlist_append(new_node, global_envlist);
}
