/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 20:58:06 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/08 20:58:26 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_heredoclist(t_heredoc *heredoclist)
{
	t_heredoc	*tmp;

	while (heredoclist)
	{
		tmp = heredoclist->next;
		if (heredoclist->input)
			free(heredoclist->input);
		free(heredoclist);
		heredoclist = tmp;
	}
	heredoclist = NULL;
}

void	free_redirlist(t_redir *redirlist)
{
	t_redir	*tmp;

	while (redirlist)
	{
		tmp = redirlist->next;
		if (redirlist->file)
			free(redirlist->file);
		if (redirlist->delimiter)
			free(redirlist->delimiter);
		if (redirlist->heredoclist)
			free_heredoclist(redirlist->heredoclist);
		free(redirlist);
		redirlist = tmp;
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->args)
			free_matrix(cmd->args);
		if (cmd->path)
			free(cmd->path);
		if (cmd->redirlist)
			free_redirlist(cmd->redirlist);
		free(cmd);
		cmd = tmp;
	}
}
