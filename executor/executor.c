/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:36:04 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:52:42 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



/*
void ignore_heredoc(char *delimiter) {
	char *line;
(void)delimiter;
	while (1) 
	{
		line = readline("...leggo il buffer heredoc: "); // Simula il prompt del heredoc
		if (!line) 
		{
			break;  // EOF, uscire dal loop
		}
		free(line); // Libera la memoria della riga
	}
}
*/

void	execute_builtin_command(t_cmd *cmd, t_env **env,
		int *exit_code, int saved_stdout)
{
	int	ret;
	int is_builtin;

	is_builtin = 1;
	ret = ihoa_redirops(cmd->redirlist, saved_stdout, is_builtin);
	if (ret == 0)
		exit(1);
	builtinex(cmd, env, exit_code);
	dup2(saved_stdout, STDOUT_FILENO);
}

void	execute_external_command(t_cmd *cmd, int saved_stdout, int *exit_code)
{
	pid_t	pid;
	int		status;
	int		ret;
	int		is_builtin;

	is_builtin = 0;

	pid = fork();
	if (pid == 0)
	{
		ret = ihoa_redirops(cmd->redirlist, saved_stdout, is_builtin);
		if (ret == 0)
			exit(1);
		execve(cmd->path, cmd->args, NULL);
		exit(1);
	}
	waitpid(pid, &status, 0);
	*exit_code = WEXITSTATUS(status);
}

void	singlecmdex(t_cmd *cmd, t_env **env, int *exit_code)
{
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (check_builtin(cmd))
	{
		execute_builtin_command(cmd, env, exit_code, saved_stdout);
		return ;
	}
	cmd->path = get_cmdpath(cmd->cmd);
	execute_external_command(cmd, saved_stdout, exit_code);
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
void	executor(t_cmd *cmdlist, t_env **env, char **envp, int *exit_code)
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
			env, envp, exit_code};
		pipex(&data);
		//exit_code = data.exit_code;
	}
	else
	{
		singlecmdex(cmdlist, env, exit_code);
		//printf("executor: sono uscito dal singlecmdex. Comando eseguito\n");
	}
}
