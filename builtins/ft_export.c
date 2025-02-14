/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:44:25 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:49:15 by largenzi         ###   ########.fr       */
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
		if (ft_strcmp(tmp_list->name, namevar) == 0)
			return (tmp_list);
		tmp_list = tmp_list->next;
	}
	return (NULL);
}

//free(envar_value);
void	update_envar_value(t_env *found_global_envar, char *envar_value)
{
	if (ft_strcmp(found_global_envar->value, envar_value) == 0)
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
