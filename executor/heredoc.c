/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:36:23 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/08 19:50:56 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_and_exit(char *inputstr, char *str_realloc)
{
	free(str_realloc);
	free(inputstr);
}

char	*allocate_heredoc_str(char *inputstr)
{
	char	*str_realloc;

	str_realloc = ft_calloc(ft_strlen(inputstr) + 2, sizeof(char));
	if (!str_realloc)
		return (NULL);
	ft_strncpy(str_realloc, inputstr, ft_strlen(inputstr));
	str_realloc[ft_strlen(inputstr)] = '\n';
	return (str_realloc);
}

void	handle_heredoc_input(t_redir *tmp_redirlist)
{
	char	*inputstr;
	char	*str_realloc;

	while (1)
	{
		inputstr = readline(">: ");
		if (!inputstr)
			return ;
		str_realloc = allocate_heredoc_str(inputstr);
		if (!str_realloc)
			return (free(inputstr));
		if (strcmp(inputstr, tmp_redirlist->delimiter) == 0)
		{
			free_and_exit(inputstr, str_realloc);
			break ;
		}
		build_heredoclist(str_realloc, &(tmp_redirlist->heredoclist));
		free_and_exit(inputstr, str_realloc);
	}
}

void	process_heredoc_redirlist(t_redir *tmp_redirlist)
{
	while (tmp_redirlist)
	{
		while (tmp_redirlist && tmp_redirlist->type != HEREDOC)
			tmp_redirlist = tmp_redirlist->next;
		if (tmp_redirlist)
		{
			handle_heredoc_input(tmp_redirlist);
			tmp_redirlist = tmp_redirlist->next;
		}
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
