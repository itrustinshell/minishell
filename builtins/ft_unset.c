/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:33:08 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:33:18 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_envar(t_env *variable, t_env **list)
{
	if (variable->next != NULL && variable->prev != NULL)
	{
		variable->prev->next = variable->next;
		variable->next->prev = variable->prev;
		free(variable);
		variable = NULL;
	}
	else if (variable->next == NULL)
	{
		variable->prev->next = NULL;
		free(variable);
		variable = NULL;
	}
	else if (variable->prev == NULL)
	{
		variable = variable->next;
		free(variable->prev);
		variable->prev = NULL;
		*list = variable;
	}
	free(variable);
}

int	ft_unset(char *namevar, t_env **list)
{
	t_env	*tmp;
	t_env	*variable;

	tmp = *list;
	variable = access_envar(namevar, tmp);
	if (variable)
		remove_envar(variable, list);
	return (1);
}
