/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:21:45 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 18:52:11 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
PROBLEMATICHE RISCONTRATE



eho

ft_ft_printf


TODO: 


- isdigit

*/

static int	g_signal_received = 0;
int			g_exit = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 1;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	g_signal_received = 2;
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	core_shell(char *inputstr, t_cmd *cmdlist, t_env **env)
{
	if (inputstr[0] != '\0' && isspace(inputstr[0]) == 0)
	{
		cmdlist = parse_input(inputstr);
		if (cmdlist)
		{
			executor(cmdlist, env);
			free_cmd(cmdlist);
			cmdlist = NULL;
			if (g_signal_received == 2)
			{
				ft_printf("Quit (core dumped)\n");
				g_exit = 131;
				return (0);
			}
		}
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*inputstr;
	t_cmd	*cmdlist;
	t_env	*env;

	unused_param(argc, argv);
	cmdlist = NULL;
	env = envcpy(envp);
	setup_signals();
	while (1)
	{
		g_signal_received = 0;
		inputstr = NULL;
		prompt(&inputstr);
		if (!inputstr)
		{
			ft_printf("\nexit\n");
			break ;
		}
		if (core_shell(inputstr, cmdlist, &env) == 0)
			break ;
		free(inputstr);
	}
	last_free(inputstr, cmdlist, env);
	rl_clear_history();
	return (g_exit);
}
