/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:06:54 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:52:43 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_echo_n(int argc, char **argv)
{
	int	i;

	i = 2;
	while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
		i++;
	while (i < argc - 1)
	{
		ft_printf("%s ", argv[i]);
		i++;
	}
	if (argv[i])
		ft_printf("%s", argv[i]);
}

/* Handle echo $? case */
void	handle_echo_exit_code(void)
{
	ft_printf("%d", g_exit);
}

/* Handle echo without -n */
void	handle_echo_default(int argc, char **argv)
{
	int	i;

	if (argc == 1)
	{
		ft_printf("\n");
		return ;
	}
	i = 1;
	while (i < argc - 1)
	{
		ft_printf("%s ", argv[i]);
		i++;
	}
	ft_printf("%s\n", argv[i]);
}

/* Main echo function */
int	ft_echo(int argc, char **argv)
{
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
		handle_echo_n(argc, argv);
	else
		handle_echo_default(argc, argv);
	return (1);
}
