/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:21:45 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 15:31:43 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <readline/history.h>

char	*create_prompt(void)
{
	char	*cwd;
	char	*cwd_mod;
	int		len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	len = ft_strlen(cwd);
	cwd_mod = malloc(len + 3);
	if (!cwd_mod)
	{
		free(cwd);
		return (NULL);
	}
	strcpy(cwd_mod, cwd);
	strcat(cwd_mod, " :");
	free(cwd);
	return (cwd_mod);
}

void	prompt(char **inputstr)
{
	char	*cwd_mod;

	cwd_mod = create_prompt();
	if (!cwd_mod)
		return ;
	*inputstr = readline(cwd_mod);
	free(cwd_mod);
	if (!*inputstr)
		return ;
	if (**inputstr)
		add_history(*inputstr);
}

void	unused_param(int argc, char **argv)
{
	(void)argv;
	(void)argc;
}

void	last_free(char *inputstr, t_cmd *cmdlist, t_env *env)
{
	if (inputstr)
		free(inputstr);
	if (cmdlist)
		free_cmd(cmdlist);
	if (env)
		free_envlist(env);
}
