/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:34:06 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:52:30 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//DA IMPLEMENTARE
int	ft_cd(char **argv)
{
	char	*cwdpath;
	char	*strmaps;
	char	*str;

	(void)str;
	(void)cwdpath;
	strmaps = argv[1];
	cwdpath = getcwd(NULL, 0);
	if (!strmaps)
		return (0);
	if (chdir(strmaps) != 0)
		ft_printf("minishell: cd: %s: No such file or directory\n", strmaps);
	str = getcwd(NULL, 0);
	return (1);
}
