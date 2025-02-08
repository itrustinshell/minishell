/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:34:08 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 19:34:20 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_heredoc(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	int		n_heredoc;
	t_redir	*tmp_redir;

	if (!cmd)
		return (0);
	tmp_cmd = cmd;
	n_heredoc = 0;
	while (tmp_cmd)
	{
		if (tmp_cmd->redirlist)
		{
			tmp_redir = tmp_cmd->redirlist;
			while (tmp_redir)
			{
				if (tmp_redir->type == HEREDOC)
					n_heredoc = n_heredoc + 1;
				tmp_redir = tmp_redir->next;
			}
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (n_heredoc);
}
