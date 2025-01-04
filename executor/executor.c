#include "../minishell.h"

int	pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix, t_env **env)
{
	int			i;
	t_command	*tmp_cmdlist;

	tmp_cmdlist = cmdlist;
	if (!tmp_cmdlist)
		return (0);
	i = -1;
	while (++i < cmdlist_len)
	{
		if (i > 0)
			tmp_cmdlist = tmp_cmdlist->next;
		pipefork(pipematrix, tmp_cmdlist, i, cmdlist_len, env);
	}
	allpipeclose(pipematrix, cmdlist_len);
	i = 0;
	while (i < cmdlist_len)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

int	builtinex(t_command *cmd, t_env **env)
{
	int	a;

	if (strcmp(cmd->cmd, "echo") == 0)
	{
		a = 0;
		while (cmd->args[a])
			a++;
		ft_echo(a, cmd->args);
		return (1);
	}
	else if (strcmp(cmd->cmd, "pwd") == 0)
		return (ft_pwd());
	else if (strcmp(cmd->cmd, "export") == 0)
	{
		if (exportcheck(cmd->args) == 1)
			ft_export(cmd->args[1], env);
	}
	else if (strcmp(cmd->cmd, "cd") == 0)
		return (ft_cd(cmd->args));
	else if (strcmp(cmd->cmd, "env") == 0)
		return (ft_env(*env));
	else if (strcmp(cmd->cmd, "exit") == 0)
	{
		ft_exit();
		return (1);
	}
	return (0);
}

void	cmdex(t_command *cmd, t_env **env)
{
	pid_t	pid;
	int		status;

	if (builtinex(cmd, env))
		return ;
	cmd->path = get_cmdpath(cmd->cmd);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd->path, cmd->args, NULL);
		exit(1);
	}
	waitpid(pid, &status, 0);
}

void	executor(t_command *cmdlist, t_env **env)
{
	int	cmdlist_len;
	int	**pipematrix;

	cmdlist_len = listlen(cmdlist);
	if (cmdlist_len > 1)
	{
		pipematrix = pipesalloc(cmdlist_len);
		pipex(cmdlist, cmdlist_len, pipematrix, env);
	}
	else
		cmdex(cmdlist, env);
}
