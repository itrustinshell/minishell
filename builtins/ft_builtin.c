#include "../minishell.h"

int check_builtin_for_singlecmd(char **matrix, t_env **global_envlist, char *inputstr)
{
	int ret;

	ret = 1;
	if (strcmp(matrix[0], "env") == 0)
		printenvlist(*global_envlist);
	else if (strcmp(matrix[0], "export") == 0)
	{
		if (check_export(matrix) == 1)
			ft_export(matrix[1], global_envlist);
	}
	else if (strcmp(matrix[0], "exit") == 0)
	{
		//ft_freematrix(matrix);
		//free(inputstr);
		(void)inputstr;
		inputstr = NULL;
		exit(1);
	}
	else
		ret = 0;
	return (ret);
}

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
