/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:23:36 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/14 19:46:50 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void static	env(char **env_var)
{
	char	*expanded;

	expanded = getenv((char *)*env_var);
	if (!expanded)
	{
		free(*env_var);
		*env_var = (char *)calloc(1, sizeof(char));
		*env_var[0] = '\0';
	}
	else
	{
		free(*env_var);
		*env_var = (char *)ft_calloc(ft_strlen(expanded) + 1, sizeof(char));
		ft_strncpy(*env_var, expanded, ft_strlen(expanded));
	}
}

void	expand(char **env_var)
{
	if (!env_var || !*env_var)
		return ;
	if (ft_strncmp(*env_var, "?", 1) == 0)
	{
		free(*env_var);
		*env_var = ft_itoa(g_exit);
	}
	else if (ft_strncmp(*env_var, "", 1) == 0)
	{
		free(*env_var);
		*env_var = ft_strdup("$");
	}
	else
		env(env_var);
}
