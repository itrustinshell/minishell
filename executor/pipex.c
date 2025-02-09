/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:46:22 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/09 19:01:53 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*Execute builtins or external command*/
void	ft_execve(t_cmd *tmp_cmdlist, t_env *genvlist,
			char **envp, int *exit_code)
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
void	handle_child_process(t_pipex_data *data, t_cmd *tmp_cmdlist, int i,
		int saved_stdout)
{
	int		ret;
	t_env	**env;
	int		is_builtin;

	is_builtin = 0;
	if (i > 0)
		piperead(data->pipematrix, i);
	if (tmp_cmdlist->next)
		pipewrite(data->pipematrix, i);
	if (check_builtin(tmp_cmdlist) == 1)
		is_builtin = 1;
	ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout, is_builtin);
	if (ret == 0)
		exit(1);
	pipeclose(data->pipematrix, data->cmdlist_len);
	env = data->env;
	ft_execve(tmp_cmdlist, *env, data->envp, data->exit_code);
	exit(1);
}

/* Fork along the pipes */
void	pipefork(t_pipex_data *data, t_cmd *cmdlist, int i)
{
	int		pid;
	int		saved_stdout;
	t_cmd	*tmp_cmdlist;

	tmp_cmdlist = cmdlist;
	pid = fork();
	saved_stdout = dup(STDOUT_FILENO);
	if (pid == 0)
		handle_child_process(data, tmp_cmdlist, i,
			saved_stdout);
}

int	check_cmdpath(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (!tmp->path)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

/*execute pipes
stats all forks in a while, then the father waits.
*/
int	pipex(t_pipex_data *data)
{
	int		i;
	t_cmd	*tmp_cmdlist;
	int		status;

	tmp_cmdlist = data->cmdlist;
	if (!tmp_cmdlist)
		return (0);
	if (check_cmdpath(data->cmdlist) == 0)
	{
		printf("Hei!! Command not found :) \n");
		return (0);
	}
	i = -1;
	while (++i < data->cmdlist_len)
	{
		if (i > 0)
			tmp_cmdlist = tmp_cmdlist->next;
		pipefork(data, tmp_cmdlist, i);
	}
	pipeclose(data->pipematrix, data->cmdlist_len);
	i = 0;
	while (i < data->cmdlist_len)
	{
		wait(&status);
		*(data)->exit_code = WEXITSTATUS(status);
		i++;
	}
	return (0);
}
