/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:39:57 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 15:47:10 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	listlenredir(t_redir *list)
{
	int		i;
	t_redir	*current;

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

t_cmd	*last_cmdnode(t_cmd *commandlist)
{
	t_cmd	*current;

	current = commandlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

t_redir	*find_last_cmdnode_redir(t_redir *commandlist)
{
	t_redir	*current;

	current = commandlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	listappend_command(t_cmd *node, t_cmd **list)
{
	t_cmd	*last_node;

	if (*list == NULL)
	{
		*list = node;
		(*list)->next = NULL;
	}
	else
	{
		last_node = last_cmdnode(*list);
		last_node->next = node;
		node->next = NULL;
	}
}

void	listappend_redir(t_redir *node, t_redir **list)
{
	t_redir	*last_node;

	if (*list == NULL)
	{
		*list = node;
		(*list)->next = NULL;
	}
	else
	{
		last_node = find_last_cmdnode_redir(*list);
		last_node->next = node;
		node->next = NULL;
	}
}
