#include "../minishell.h"

/*exec builtins or external cmd*/
void ft_execve(t_command *tmp_cmdlist, t_env *genvlist)
{
	if (builtinex(tmp_cmdlist, &genvlist) == 1)
		return;
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	char **envlist = litoma(genvlist);
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);
}

/*fork along the pipes*/
void	pipefork(int **pipematrix,t_command *tmp_cmdlist, int i, int cmdlist_len, t_env **env)
{
	int pid;
	int saved_stdout;

	pid = fork();
	saved_stdout = dup(STDOUT_FILENO);
	if (pid == 0) 
	{
		if (i > 0) 
		{	
			piperead(pipematrix, i);
			if (ioa_ops(tmp_cmdlist->redirlist, saved_stdout) == 0)
				exit(1);
		}
		if (tmp_cmdlist->next) 
		{ 
			pipewrite(pipematrix, i);
			if (ioa_ops(tmp_cmdlist->redirlist, saved_stdout) == 0)
				exit(1);
		}
		allpipeclose(pipematrix, cmdlist_len);
		ft_execve(tmp_cmdlist, *env);
		exit(1);
	}
}
