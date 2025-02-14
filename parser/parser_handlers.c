/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:22:50 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/14 19:23:54 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirection(char **command_string, t_tkn *token)
{
	if (**command_string == '<')
	{
		if (*(*command_string + 1) == '<')
			token->len = 2;
		else
			token->len = 1;
		token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
		ft_strncpy(token->value, *command_string, token->len);
		*command_string += token->len;
	}
	else if (**command_string == '>')
	{
		if (*(*command_string + 1) == '>')
			token->len = 2;
		else
			token->len = 1;
		token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
		ft_strncpy(token->value, *command_string, token->len);
		*command_string += token->len;
	}
}

void	handle_quotes(char **command_string, t_tkn *token)
{
	char	*beginning;
	char	*expanded;

	beginning = ++(*command_string);
	while (**command_string && **command_string != '"')
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
	expanded = handle_env_vars(token->value);
	free(token->value);
	token->value = expanded;
	token->len = ft_strlen(expanded);
	(*command_string)++;
}

void	handle_squotes(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = ++(*command_string);
	while (**command_string && **command_string != '\'')
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
	(*command_string)++;
}

void	handle_env_var(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = ++(*command_string);
	while (**command_string && is_env_char(**command_string))
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
	expand(&token->value);
}

void	handle_plain_text(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = *command_string;
	while (is_valid_arg_char(**command_string) && **command_string)
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
}
