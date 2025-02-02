/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:55:53 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:59:02 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_prompt(char **inputstr)
{
	size_t	len;

	len = 0;
	printf(">: ");
	getline(inputstr, &len, stdin);
}

void	heredocinit(t_heredoc *node)
{
	if (node)
	{
		node->input = NULL;
		node->next = NULL;
	}
}

t_heredoc	*create_heredocnode(char *inputstr)
{
	t_heredoc	*node;

	if (!inputstr)
		return (NULL);
	node = (t_heredoc *)malloc(sizeof(t_heredoc));
	heredocinit(node);
	node->input = strdup(inputstr);
	return (node);
}

t_heredoc	*last_heredocnode(t_heredoc *list)
{
	while (list && list->next)
		list = list->next;
	return (list);
}

void	listappend_heredoc(t_heredoc *node, t_heredoc **list)
{
	t_heredoc	*last_node;

	if (!*list)
		*list = node;
	else
	{
		last_node = last_heredocnode(*list);
		last_node->next = node;
	}
}
