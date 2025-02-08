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
		line = readline("...leggo il buffer heredoc: ");
		if (!line) 
		{
			break;  // EOF, uscire dal loop
		}
		free(line); // Libera la memoria della riga
	}
}
*/
void	execute_builtin(t_cmd *cmd, t_env **env, int *exit_code, int sstdout)
{
	int	ret;

	ret = ihoa_redirops(cmd->redirlist, sstdout, TRUE);
	if (ret == 0)
		exit(1);
	builtinex(cmd, env, exit_code);
	dup2(sstdout, STDOUT_FILENO);
}

void	execute_external_command(t_cmd *cmd, int saved_stdout, int *exit_code)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		ret = ihoa_redirops(cmd->redirlist, saved_stdout, FALSE);
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
		execute_builtin(cmd, env, exit_code, saved_stdout);
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

void free_pipes(int **pipematrix, int num_of_pipes)
{
    int i;

    if (!pipematrix)
        return;
    for (i = 0; i < num_of_pipes; i++)
    {
        free(pipematrix[i]);
    }
    free(pipematrix);
}


void	executor(t_cmd *cmdlist, t_env **env, char **envp, int *exit_code)
{
	int				cmdlist_len;
	int				**pipematrix;
	int				n_heredoc;
	t_pipex_data	data;

	n_heredoc = count_heredoc(cmdlist);
	heredoc(cmdlist, n_heredoc);
	cmdlist_len = listlen(cmdlist);
	ft_printf("lunghezaaaaaa: %d\n", cmdlist_len);
	if (cmdlist_len == 0)
		return ;
	if (cmdlist_len > 1)
	{
		pipematrix = pipesalloc(cmdlist_len);
		data = (t_pipex_data){cmdlist, cmdlist_len, pipematrix,
			env, envp, exit_code};
		pipex(&data);
		free_pipes(pipematrix, cmdlist_len - 1);
	}
	else
	{
		singlecmdex(cmdlist, env, exit_code);
	}
}
