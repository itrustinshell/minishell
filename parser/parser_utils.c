/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:09:14 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/06 14:10:27 by dpalmese         ###   ########.fr       */
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
