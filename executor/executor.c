/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:36:04 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 16:39:08 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_builtin(t_cmd *cmd, t_env **env, int sstdout)
{
	int	ret;

	ret = ihoa_redirops(cmd->redirlist, sstdout, TRUE);
	if (ret == 0)
		exit(1);
	builtinex(cmd, env);
	dup2(sstdout, STDOUT_FILENO);
}

void	execute_external_command(t_cmd *cmd, int saved_stdout, t_env **env)
{
	pid_t	pid;
	int		status;
	int		ret;
	char	**envlist;

	pid = fork();
	if (pid == 0)
	{
		ret = ihoa_redirops(cmd->redirlist, saved_stdout, FALSE);
		if (ret == 0)
			exit(1);
		envlist = litoma(*env);
		execve(cmd->path, cmd->args, envlist);
		exit(1);
	}
	waitpid(pid, &status, 0);
	g_exit = WEXITSTATUS(status);
}

void	singlecmdex(t_cmd *cmd, t_env **env)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (check_builtin(cmd))
	{
		execute_builtin(cmd, env, saved_stdout);
		return ;
	}
	cmd->path = get_cmdpath(cmd->cmd);
	if (!cmd->path)
	{
		printf("command not found\n");
		return ;
	}
	execute_external_command(cmd, saved_stdout, env);
}

/*	
	DESCRIPTION
		execute cmdlist
	
	UTILITY
		usefull functions for debug:
		- printlist(cmdlist);
		- printf("n_heredoc: %d\n", n_heredoc);
		- printallheredoclists(cmdlist, n_heredoc);
*/
void	free_pipes(int **pipematrix, int num_of_pipes)
{
	int	i;

	i = 0;
	if (!pipematrix)
		return ;
	while (i < num_of_pipes)
	{
		free(pipematrix[i]);
		i++;
	}
	free(pipematrix);
}

void	executor(t_cmd *cmdlist, t_env **env)
{
	int				cmdlist_len;
	int				**pipematrix;
	int				n_heredoc;
	t_pipex_data	data;

	n_heredoc = count_heredoc(cmdlist);
	heredoc(cmdlist, n_heredoc);
	cmdlist_len = listlen(cmdlist);
	if (cmdlist_len == 0)
		return ;
	if (cmdlist_len > 1)
	{
		pipematrix = pipesalloc(cmdlist_len);
		data = (t_pipex_data){cmdlist, cmdlist_len, pipematrix,
			env};
		pipex(&data);
		free_pipes(pipematrix, cmdlist_len - 1);
	}
	else
	{
		singlecmdex(cmdlist, env);
	}
}
