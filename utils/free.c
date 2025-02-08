/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:40:43 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/08 22:40:46 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tkn(void *token)
{
	t_tkn	*t;

	t = (t_tkn *)token;
	if (t)
	{
		free(t->value);
		free(t);
	}
}

void	free_tokenslist(t_list *tokens)
{
	t_list	*tmp;
	t_tkn	*token;

	while (tokens)
	{
		tmp = tokens->next;
		token = (t_tkn *)tokens->content;
		if (token)
		{
			free(token->value);
			free(token);
		}
		free(tokens);
		tokens = tmp;
	}
}

void	ft_free_n_matrix(char **matrix, int n)
{
	int	j;

	j = 0;
	while (j < n)
	{
		free(matrix[j]);
		j++;
	}
	free(matrix);
}

void	free_matrix(char **matrix)
{
	int	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i++;
	}
	free(matrix);
	matrix = NULL;
}

void	free_envlist(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}
