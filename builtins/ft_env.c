/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:43:12 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:57:10 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env *lenvlist)
{
	t_env	*tmp_envlist;

	if (!lenvlist)
		return (0);
	tmp_envlist = lenvlist;
	while (tmp_envlist)
	{
		ft_printf("%s=%s\n", tmp_envlist->name, tmp_envlist->value);
		tmp_envlist = tmp_envlist->next;
	}
	return (1);
}

t_env	*envcpy(char **envp)
{
	t_env	*envlist;
	t_env	*envnode;
	int		i;

	envlist = NULL;
	i = 0;
	while (envp[i])
	{
		envnode = create_lenvnode(envp[i]);
		if (!envnode)
		{
			free_envlist(envlist);
			return (NULL);
		}
		envlist_append(envnode, &envlist);
		i++;
	}
	return (envlist);
}
