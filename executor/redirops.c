/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:47:13 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 19:53:00 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* esegue le operazioni oa */
void	oa_redirops(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist || oa_rediropen(redirlist) == 0)
		return ;
	redirnode = oa_redirlast(redirlist);
	if (!redirnode)
		return ;
	oa_redirwrite(redirnode);
}

int	handle_input_redirection(t_redir *latest_input_redir, int saved_stdout)
{
	int	fd;

	if (access(latest_input_redir->file, F_OK) == 0)
	{
		fd = open(latest_input_redir->file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (3);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	printf("ERROREEEEEEEEE\n");
	return (0);
}

int	handle_heredoc(t_redir *latest_input_redir, int is_builtin)
{
	int			fdpipe[2];
	t_heredoc	*current;
	pipe(fdpipe);
	if (latest_input_redir->heredoclist)
	{
		current = latest_input_redir->heredoclist;
		while (current)
		{
			write(fdpipe[1], current->input, strlen(current->input));
			current = current->next;
		}
		close(fdpipe[1]);
	}
	if (is_builtin == 0)
		dup2(fdpipe[0], STDIN_FILENO);
	close(fdpipe[0]);
	return (4);
}

int	ih_redirops(t_redir *redirlist, int saved_stdout, int is_builtin)
{
	t_redir	*latest_input_redir;

	if (!redirlist)
		return (2);
	latest_input_redir = ih_redirlast(redirlist);
	if (!latest_input_redir)
		return (2);
	if (latest_input_redir->type == INPUT_REDIRECTION)
		return (handle_input_redirection(latest_input_redir, saved_stdout));
	else if (latest_input_redir->type == HEREDOC)
		return (handle_heredoc(latest_input_redir, is_builtin));
	return (1);
}

/* O'mast of  input-heredoc-output-append (IHOA) operations */
int	ihoa_redirops(t_redir *redirlist, int saved_stdout, int is_builtin)
{
	int	ih_ret;

	ih_ret = ih_redirops(redirlist, saved_stdout, is_builtin);
	if (ih_ret == 0)
		return (0);
	oa_redirops(redirlist);
	return (ih_ret);
}
