/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeutils_p2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:37:04 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 23:12:32 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Execute builtins or external command*/
void	ft_execve(t_cmd *tmp_cmdlist, t_env *genvlist, char **envp, int *exit_code)
{
	char	**envlist;

	if (builtinex(tmp_cmdlist, &genvlist, exit_code) == 1)
		return ;
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	envlist = litoma(genvlist);
	(void)envp;
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);
}

/* Handle child process execution */
void	handle_child_process(int **pipematrix, t_cmd *tmp_cmdlist, int i,
		int cmdlist_len, int saved_stdout, t_env **env, char **envp, int *exit_code)
{
	int	n_heredoc;
	int	ret;

	n_heredoc = 0;
	if (i > 0)
	{
		piperead(pipematrix, i);
		ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
		if (ret == 0)
			exit(1);
	}
	if (tmp_cmdlist->next)
	{
		pipewrite(pipematrix, i);
		ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
		printf("Sono stati rilevati %d heredoc\n", n_heredoc);
		if (ret == 0)
			exit(1);
	}
	pipeclose(pipematrix, cmdlist_len);
	ft_execve(tmp_cmdlist, *env, envp, exit_code);
	exit(1);
}

/* Fork along the pipes */
void	pipefork(int **pipematrix, t_cmd *cmdlist, int i,
		int cmdlist_len, t_env **env, char **envp, int *exit_code)
{
	int		pid;
	int		saved_stdout;
	t_cmd	*tmp_cmdlist;

	tmp_cmdlist = cmdlist;
	pid = fork();
	saved_stdout = dup(STDOUT_FILENO);
	if (pid == 0)
		handle_child_process(pipematrix, tmp_cmdlist, i, cmdlist_len,
			saved_stdout, env, envp, exit_code);
}
