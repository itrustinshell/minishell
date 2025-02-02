/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:44:13 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:52:09 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(int n_args, char **args)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	if (n_args > 2)
	{
		printf("minishell: exit: too many arguments\n");
		return ;
	}
	if (n_args == 2)
	{
		i = 0;
		while (args[1][i])
		{
			if (isdigit(args[1][i]) == 0)
			{
				printf("minishell: exit: ee: numeric argument required\n");
				return ;
			}
			i++;
		}
		exit_code = atoi(args[1]);
	}
	exit(exit_code);
}
