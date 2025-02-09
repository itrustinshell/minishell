/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:09:14 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/09 14:57:31 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tkn_is_redir(t_tkn *token)
{
	int		result;
	int		len;
	char	*value;

	len = token->len;
	value = token->value;
	result = FALSE;
	if (len == 2)
	{
		result = ft_strncmp(value, "<<", 2) == 0;
		result = result || ft_strncmp(value, ">>", 2) == 0;
	}
	else if (len == 1)
	{
		result = ft_strncmp(value, "<", 1) == 0;
		result = result || ft_strncmp(token->value, ">", 1) == 0;
	}
	return (result);
}

void	set_redir_tkn(t_tkn *token)
{
	int		len;
	char	*value;

	len = token->len;
	value = token->value;
	if (len == 2 && ft_strncmp(value, "<<", 2) == 0)
		token->type = DOUBLE_LEFT;
	else if (len == 2 && ft_strncmp(value, ">>", 2) == 0)
		token->type = DOUBLE_RIGHT;
	else if (len == 1 && ft_strncmp(value, "<", 1) == 0)
		token->type = LEFT;
	else if (len == 1 && ft_strncmp(value, ">", 1) == 0)
		token->type = RIGHT;
}
/*
int	count_args(t_list *tokens)
{
	t_tkn	*current_tok;
	int		arguments;

	arguments = 0;
	while (tokens)
	{
		current_tok = (t_tkn *)tokens->content;
		if (current_tok->type == ARG)
			arguments++;
		tokens = tokens->next;
	}
	return (arguments);
}

void	print_redir(void *redir_node)
{
	printf("%s\n", ((t_redir *)redir_node)->file);
}

BOOL	is_valid_arg_char(char c)
{
	return (!ft_isspace(c) && !ft_strchr("|<>\"\'$", c));
}
*/