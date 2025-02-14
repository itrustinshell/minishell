/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:44:13 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:53:32 by largenzi         ###   ########.fr       */
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
		ft_printf("minishell: exit: too many arguments\n");
		return ;
	}
	if (n_args == 2)
	{
		i = 0;
		while (args[1][i])
		{
			if (ft_isdigit(args[1][i]) == 0)
			{
				ft_printf("exit\nminishell: ");
				ft_printf("exit: %s: numeric argument required\n", args[1]);
				return ;
			}
			i++;
		}
		exit_code = ft_atoi(args[1]);
	}
	exit(exit_code);
}
