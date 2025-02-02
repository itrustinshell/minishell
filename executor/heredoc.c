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

void	build_heredoclist(char *inputstr, t_heredoc **heredoclist)
{
	t_heredoc	*node;

	node = create_heredocnode(inputstr);
	listappend_heredoc(node, heredoclist);
}

void	handle_heredoc_input(t_redir *tmp_redirlist)
{
	char	*inputstr;
	size_t	len;
	int		j;

	len = 0;
	while (1)
	{
		printf(">: ");
		getline(&inputstr, &len, stdin);
		j = 0;
		while (inputstr[j] != '\n')
			j++;
		inputstr[j] = '\0';
		if (strcmp(inputstr, tmp_redirlist->delimiter) == 0)
			break ;
		inputstr[j] = '\n';
		build_heredoclist(inputstr, &(tmp_redirlist->heredoclist));
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
