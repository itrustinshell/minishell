/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:06:54 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/09 14:35:43 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_echo_n(int argc, char **argv)
{
	int	i;

	i = 2;
	while (argv[i] && strcmp(argv[i], "-n") == 0)
		i++;
	while (i < argc - 1)
	{
		printf("%s ", argv[i]);
		i++;
	}
	if (argv[i])
		printf("%s", argv[i]);
}

/* Handle echo $? case */
void	handle_echo_exit_code()
{
	printf("%d", g_exit);
}

/* Handle echo without -n */
void	handle_echo_default(int argc, char **argv)
{
	int	i;

	if (argc == 1)
	{
		printf("\n");
		return ;
	}
	i = 1;
	while (i < argc - 1)
	{
		if (argv[i][0] == '$' && argv[i][1] == '?')
			printf("%d ", g_exit);
		else
			printf("%s ", argv[i]);
		i++;
	}
	if (argv[i][0] == '$' && argv[i][1] == '?')
		printf("%d\n", g_exit);
	else
		printf("%s\n", argv[i]);
}

/* Main echo function */
int	ft_echo(int argc, char **argv)
{
	if (argv[1] && strcmp(argv[1], "-n") == 0)
		handle_echo_n(argc, argv);
	else if (argc == 2 && strcmp(argv[1], "$?") == 0)
		handle_echo_exit_code();
	else
		handle_echo_default(argc, argv);
	return (1);
}
