/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:18:33 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/13 12:20:38 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

static char	*join_and_free(char *s1, char *s2, int free_s2)
{
	char	*result;

	if (!s1)
	{
		if (free_s2)
			return (NULL);
		else
			return (s2);
	}
	result = ft_strjoin(s1, s2);
	free(s1);
	if (free_s2)
		free(s2);
	return (result);
}

static char	*copy_until_dollar(const char **str)
{
	const char	*dollar;
	char		*result;

	dollar = ft_strchr(*str, '$');
	if (!dollar)
	{
		result = ft_strdup(*str);
		*str += ft_strlen(*str);
		return (result);
	}
	result = ft_substr(*str, 0, dollar - *str);
	*str = dollar;
	return (result);
}

static char	*get_env_var(const char **str)
{
	char	*var_name;
	size_t	len;

	(*str)++;
	len = 0;
	if ((*str)[len] == '?')
	{
		//printf("eccp %c\n",(*str)[len]);
		char *exit_str = ft_itoa(g_exit);
		//printf("questa e'la var glob: %d\n", g_exit);
		(*str)++;
		return ft_strdup(exit_str);
	}	
	
	//printf("%c\n", (*str)[len]);

	while ((*str)[len] && is_env_char((*str)[len]))
		len++;
	//printf("%c\n", (*str)[len]);


	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(*str, 0, len);
	*str += len;
	if (var_name)
		expand(&var_name);
	return (var_name);
}

char	*handle_env_vars(const char *input)
{
	char		*result;
	char		*tmp;
	const char	*current;

	if (!input)
		return (NULL);
	result = ft_strdup("");
	current = input;
	while (*current)
	{
		tmp = copy_until_dollar(&current);
		result = join_and_free(result, tmp, 1);
		if (*current == '$')
		{
			tmp = get_env_var(&current);
			result = join_and_free(result, tmp, 1);
		}
	}
	return (result);
}
