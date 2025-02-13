/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokenize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:21:11 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/13 12:21:23 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

BOOL	has_command(t_list *tokens)
{
	t_list	*node;

	if (!tokens)
		return (FALSE);
	node = tokens;
	while (node)
	{
		if (((t_tkn *)(node->content))->type == COMMAND)
			return (TRUE);
		node = node->next;
	}
	return (FALSE);
}

void	get_token_type(t_tkn *token, t_list *tokens)
{
	t_list			*last_node;
	enum token_type	type;

	if (tkn_is_redir(token))
		set_redir_tkn(token);
	else
	{
		last_node = ft_lstlast(tokens);
		if (last_node)
			type = ((t_tkn *)last_node->content)->type;
		if (last_node && type == LEFT)
			token->type = LEFT_ARG;
		else if (last_node && (type == DOUBLE_LEFT))
			token->type = HEREDOC_ARG;
		else if (last_node && type == RIGHT)
			token->type = RIGHT_ARG;
		else if (last_node && type == DOUBLE_RIGHT)
			token->type = APPEND_ARG;
		else if (last_node && (type == COMMAND || type == ARG))
			token->type = ARG;
		else if (!last_node || !has_command(tokens))
			token->type = COMMAND;
	}
}

t_tkn	*create_token(void)
{
	t_tkn	*token;

	token = (t_tkn *)ft_calloc(1, sizeof(t_tkn));
	if (!token)
		return (NULL);
	return (token);
}

t_list	*tokenize(char *command_string)
{
	t_tkn	*token;
	t_list	*tokens;

	tokens = NULL;
	while (*command_string)
	{
		while (ft_isspace(*command_string))
			command_string++;
		if (!*command_string)
			break ;
		token = create_token();
		if (*command_string == '<' || *command_string == '>')
			handle_redirection(&command_string, token);
		else if (*command_string == '"')
			handle_quotes(&command_string, token);
		else if (*command_string == '\'')
			handle_squotes(&command_string, token);
		else if (*command_string == '$')
			handle_env_var(&command_string, token);
		else
			handle_plain_text(&command_string, token);
		get_token_type(token, tokens);
		ft_lstadd_back(&tokens, ft_lstnew(token));
	}
	return (tokens);
}
