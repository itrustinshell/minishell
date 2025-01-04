#include "../minishell.h"




int check_builtin_in_cmdlist(t_command *tmp_cmdlist, t_env *genvlist)
{
	if (strcmp(tmp_cmdlist->cmd, "echo") == 0)
	{
		int a = 0;
		while (tmp_cmdlist->args[a])
			a++;
		ft_echo(a, tmp_cmdlist->args);
		return (1);
	}
	else if (strcmp(tmp_cmdlist->cmd, "pwd") == 0)
		return (ft_pwd());
	else if (strcmp(tmp_cmdlist->cmd, "cd") == 0)
		return (ft_cd(tmp_cmdlist->args));
	else if (strcmp(tmp_cmdlist->cmd, "env") == 0)
		return (printenvlist(genvlist));
	else if (strcmp(tmp_cmdlist->cmd, "exit") == 0)
	{
		ft_exit();
		return 1;
	}
	return (0);
}
