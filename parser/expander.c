/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:23:36 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/13 12:24:00 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand(char **env_var)
{
	char	*expanded;

	if (!env_var || !*env_var)
		return ;
	free(*env_var);
	if (ft_strncmp(*env_var, "?", 1) == 0)
		*env_var = ft_itoa(g_exit);
	else if (ft_strncmp(*env_var, "", 1) == 0)
		*env_var = ft_strdup("$");
	else
	{
		expanded = getenv((char *)*env_var);
		if (!expanded)
		{
			*env_var = (char *)calloc(1, sizeof(char));
			*env_var[0] = '\0';
		}
		else
		{
			*env_var = (char *)ft_calloc(ft_strlen(expanded) + 1, sizeof(char));
			ft_strncpy(*env_var, expanded, ft_strlen(expanded));
		}
	}
}
