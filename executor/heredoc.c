/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:36:23 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:58:58 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_heredoc_input(t_redir *tmp_redirlist)
{
	char	*inputstr;
	char	*str_realloc;

	while (1)
	{
		inputstr = readline(">: ");
		if (!inputstr)
			return ;
		str_realloc = ft_calloc((ft_strlen(inputstr) + 1), sizeof(char));
		ft_strncpy(str_realloc, inputstr, ft_strlen(inputstr));
		str_realloc[ft_strlen(inputstr)] = '\n';
		if (strcmp(inputstr, tmp_redirlist->delimiter) == 0)
			break ;
		build_heredoclist(str_realloc, &(tmp_redirlist->heredoclist));
	}
}

void	process_heredoc_redirlist(t_redir *tmp_redirlist)
{
	while (tmp_redirlist)
	{
		while (tmp_redirlist && tmp_redirlist->type != HEREDOC)
			tmp_redirlist = tmp_redirlist->next;
		if (tmp_redirlist)
			handle_heredoc_input(tmp_redirlist);
		tmp_redirlist = tmp_redirlist->next;
	}
}

void	heredoc(t_cmd *cmd, int n_heredoc)
{
	t_cmd	*tmp_cmdlist;

	tmp_cmdlist = cmd;
	if (!n_heredoc)
		return ;
	while (tmp_cmdlist)
	{
		process_heredoc_redirlist(tmp_cmdlist->redirlist);
		tmp_cmdlist = tmp_cmdlist->next;
	}
}
