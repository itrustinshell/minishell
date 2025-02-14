/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:38:29 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 15:38:53 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Inizializza la struttura t_cmd
t_redir				*redirlist;c;
 */

t_cmd	*new_cmd(unsigned int argc)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (argc > 0)
	{
		cmd->args = (char **)ft_calloc(argc, sizeof(char *));
		if (!cmd->args)
		{
			free(cmd);
			return (NULL);
		}
	}
	argc = 0;
	return (cmd);
}

/* Inizializza la struttura t_redir */
void	redirinit(t_redir *node)
{
	node->type = 0;
	node->file = NULL;
	node->delimiter = NULL;
	node->heredoclist = NULL;
	node->next = NULL;
}
