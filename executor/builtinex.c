/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtinex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:44:00 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:52:26 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_builtin(t_cmd *cmd)
{
	if (strcmp(cmd->cmd, "echo") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "pwd") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "export") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "cd") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "env") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "exit") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "unset") == 0)
		return (1);
	else
		return (0);
}

int	arguments_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	handle_exit(t_cmd *cmd)
{
	ft_exit(arguments_count(cmd->args), cmd->args);
	return (1);
}

int	builtinex(t_cmd *cmd, t_env **env, int *exit_code)
{
	if (strcmp(cmd->cmd, "echo") == 0)
	{
		ft_echo(arguments_count(cmd->args), cmd->args, exit_code);
		return (1);
	}
	if (strcmp(cmd->cmd, "pwd") == 0)
		return (ft_pwd());
	if (strcmp(cmd->cmd, "export") == 0 && exportcheck(cmd->args))
		ft_export(cmd->args[1], env);
	if (strcmp(cmd->cmd, "cd") == 0)
		return (ft_cd(cmd->args));
	if (strcmp(cmd->cmd, "env") == 0)
		return (ft_env(*env));
	if (strcmp(cmd->cmd, "exit") == 0)
		return (handle_exit(cmd));
	if (strcmp(cmd->cmd, "unset") == 0)
	{
		ft_unset(cmd->args[1], env);
		return (1);
	}
	return (0);
}
