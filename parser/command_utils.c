/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:21:45 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/13 12:21:51 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*new_redir(char *filename, int type)
{
	t_redir	*redir;

	redir = (t_redir *)calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = filename;
	redir->next = NULL;
	return (redir);
}

// Se non riusciamo ad allocare la stringa per il comando Amen
void	add_cmd(t_cmd *cmd, t_tkn *token)
{
	cmd->cmd = (char *)calloc(token->len + 1, sizeof(char));
	ft_strncpy(cmd->cmd, token->value, token->len);
	cmd->path = get_cmdpath(cmd->cmd);
	cmd->args[0] = ft_strdup(cmd->cmd);
}

void	add_arg(t_cmd *cmd, t_tkn *token)
{
	cmd->args[cmd->argc + 1] = calloc(token->len + 1, sizeof(char));
	ft_strncpy(cmd->args[cmd->argc + 1], token->value, token->len);
	cmd->argc++;
}

void	add_redir(t_cmd *cmd, t_tkn *token, unsigned int type)
{
	t_redir	*redir;

	redir = new_redir((char *)calloc(token->len + 1, sizeof(char)), type);
	ft_strncpy(redir->file, token->value, token->len);
	listappend_redir(redir, &cmd->redirlist);
}

void	add_heredoc(t_cmd *cmd, t_tkn *token)
{
	t_redir	*redir;

	redir = new_redir(NULL, HEREDOC);
	redir->delimiter = (char *)calloc(token->len + 1, sizeof(char));
	ft_strncpy(redir->delimiter, token->value, token->len);
	listappend_redir(redir, &cmd->redirlist);
}
