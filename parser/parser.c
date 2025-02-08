/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:29:03 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/08 20:19:30 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "../minishell.h"

void	assign_command_or_arg(t_cmd *cmd, t_tkn *curr_tok)
{
	if (curr_tok->type == COMMAND)
		add_cmd(cmd, curr_tok);
	else if (curr_tok->type == ARG)
		add_arg(cmd, curr_tok);
}

void	assign_redirection(t_cmd *cmd, t_tkn *curr_tok)
{
	if (curr_tok->type == LEFT_ARG)
		add_redir(cmd, curr_tok, INPUT_REDIRECTION);
	else if (curr_tok->type == RIGHT_ARG)
		add_redir(cmd, curr_tok, OUTPUT_REDIRECTION);
	else if (curr_tok->type == APPEND_ARG)
		add_redir(cmd, curr_tok, APPEND_REDIRECTION);
	else if (curr_tok->type == HEREDOC_ARG)
		add_heredoc(cmd, curr_tok);
}

t_cmd	*create_command(t_list *tokens)
{
	t_list	*current_node;
	t_tkn	*curr_tok;
	t_cmd	*cmd;
	int		arg_count;

	arg_count = count_args(tokens);
	cmd = new_cmd(arg_count + 2);
	if (!cmd)
		return (NULL);
	cmd->args[arg_count + 1] = NULL;
	current_node = tokens;
	while (current_node)
	{
		curr_tok = (t_tkn *)current_node->content;
		assign_command_or_arg(cmd, curr_tok);
		assign_redirection(cmd, curr_tok);
		current_node = current_node->next;
	}
	return (cmd);
}

t_cmd	*parse_command(char *command_string)
{
	t_list	*tokens;
	t_cmd	*cmd;

	tokens = tokenize(command_string);
	cmd = create_command(tokens);
	free_tokenslist(tokens);
	return (cmd);
}

t_cmd	*parse_input(char *input)
{
	char	**command_strings;
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;

	cmd_list = NULL;
	command_strings = ft_split(input, PIPE); //TODO: use the right split
	if (!command_strings)
		return (NULL);
	i = 0;
	while (command_strings[i])
	{
		current_cmd = parse_command(command_strings[i]);
		if (!current_cmd)
		{
			free_matrix(command_strings);
			free_cmd(cmd_list);
			return (NULL);
		}
		listappend_command(current_cmd, &cmd_list);
		printf("Oilloc o comand :%s %s\n", current_cmd->cmd, current_cmd->args[1]);
		i++;
	}
	free_matrix(command_strings);
	return (cmd_list);
}
