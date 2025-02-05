#include "../minishell.h"

void	ft_free_n_matrix(char **matrix, int n)
{
	int	j;

	j = 0;
	while (j < n)
	{
		free(matrix[j]);
		j++;
	}
	free(matrix);
}

void	free_matrix(char **matrix)
{
	int	i;

	if (!matrix)
		return ;		
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i++;
	}
	free(matrix);
	matrix = NULL;
}


void	free_envlist(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_cmd(t_cmd *cmd) //solo quando è singolo comando....uindi non frea le liste di comandi....non fria i next...aaa dovrebbe friare la lista di redir perchè è fissa per  ogni comando...da implementare
{
	if (cmd)
	{
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->args)
			free_matrix(cmd->args);
		if (cmd->path)
			free(cmd->path);
		free(cmd);
	}
}



/*

	char				*cmd;
	char				**args;
	char				*path;
	struct s_cmd		*next;
	t_redir				*redirlist;
	int					argc;


*/


// freeredir()
// {

// }

// void	free_redirlist(t_redir *redirnode)
// {
// 	if (!redirnode)
// 		return;
// 	while (redirnode)
// 	{
// 		freeredir(redirnode);
// 		redirnode = redirnode->next;
// 	}
// }


// void	free_cmd(cmd)
// {
// 	if (!cmd)
// 		return;
// 	free(cmd->cmd);
// 	if (cmd->args)
// 		ft_freematrix(cmd->args);
// 	if (cmd->path)
// 		free(cmd->path);
// 	if (cmd->redirlist)
// 		free_redirlist(cmd->redirlist);
// }

// void	free_cmdlist(t_cmd *cmdnode)
// {

// 	if (!cmdnode)
// 		return;
// 	while (cmdnode)
// 	{
// 		freecmd(cmdnode);
// 		cmdnode = cmdnode->next;
// 	}
// }
