#include "../minishell.h"

/*execute pipes
stats all forks in a while, then the father waits.
*/

int	pipex(t_pipex_data *data, int *exit_code)
{
	int		i;
	t_cmd	*tmp_cmdlist;
	int		status;

	tmp_cmdlist = data->cmdlist;
	if (!tmp_cmdlist)
		return (0);
	i = -1;
	while (++i < data->cmdlist_len)
	{
		if (i > 0)
			tmp_cmdlist = tmp_cmdlist->next;
		pipefork(data->pipematrix, tmp_cmdlist, 
		 i,  data->cmdlist_len,  data->env,  data->envp,  exit_code);
	}
	pipeclose(data->pipematrix, data->cmdlist_len);
	i = 0;
	while (i < data->cmdlist_len)
	{
		wait(&status);
		*exit_code = WEXITSTATUS(status);
		i++;
	}
	return (0);
}

int	arguments_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/*execute builtins*/
int	builtinex(t_cmd *cmd, t_env **env, int *exit_code)
{
	if (strcmp(cmd->cmd, "echo") == 0)
	{
		ft_echo(arguments_count(cmd->args), cmd->args, exit_code);
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
		ft_exit(arguments_count(cmd->args), cmd->args);
		return (1);
	}
	else if (strcmp(cmd->cmd, "unset") == 0)
	{
		ft_unset(cmd->args[1], env);
		return (1);
	}
	return (0);
}

int	check_builtin(t_cmd *cmd)
{
	if (strcmp(cmd->cmd, "echo") == 0)
			return (1);
	else if (strcmp(cmd->cmd, "pwd") == 0)
			return (1);
	else if (strcmp(cmd->cmd, "export") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "cd") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "env") == 0)
		return (1);
	else if (strcmp(cmd->cmd, "exit") == 0)
		return (1);
	else if(strcmp(cmd->cmd, "unset") == 0)
		return (1);
	else
		return (0);
}

/*
	DESCRIPTION
		execute single command it first try for builtin.
		If there are no builtin it  continues executind 
		external command.
*/
void	cmdex(t_cmd *cmd, t_env **env, int *exit_code)
{
	pid_t	pid;
	int		status;
	int		ret;
	int		saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (check_builtin(cmd))
	{
		ret = ihoa_redirops(cmd->redirlist, saved_stdout);
		if (ret == 0)
			exit(1);
		builtinex(cmd, env, exit_code);
		dup2(saved_stdout, STDOUT_FILENO);
		return ;
	}
	cmd->path = get_cmdpath(cmd->cmd);
	pid = fork();
	if (pid == 0)
	{
		ret = ihoa_redirops(cmd->redirlist, saved_stdout);	
		if (ret == 0)
			exit(1);
		execve(cmd->path, cmd->args, NULL);
		exit(1);
	}
	waitpid(pid, &status, 0);
	*exit_code = WEXITSTATUS(status);
}

void heredoc_prompt(char **inputstr)
{
	size_t len;
	len = 0;

	//*inputstr = NULL;
	printf(">: ");
	getline(inputstr, &len, stdin); //free di inputstr fatto!
}

void	heredocinit(t_heredoc *node)
{
	if (node)
	{
		node->input = NULL;
		node->next = NULL;
	}
}

t_heredoc *create_heredocnode(char *inputstr)
{
	t_heredoc	*node;

	if(!inputstr)
		return (NULL);
	node = (t_heredoc *)malloc(sizeof(t_heredoc));
	heredocinit(node);
	node->input = strdup(inputstr);
	return (node);
}

t_heredoc	*last_heredocnode(t_heredoc *list)
{
	t_heredoc	*current;

	current = list;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	listappend_heredoc(t_heredoc *node, t_heredoc **list)
{
	t_heredoc	*last_node;

	if (*list == NULL)
	{
		*list = node;
		(*list)->next = NULL;
	}
	else
	{
		last_node = last_heredocnode(*list);
		last_node->next = node;
		node->next = NULL;
	}
}

void	build_heredoclist(char *inputstr, t_heredoc **heredoclist)
{
	t_heredoc	*node;

	(void)inputstr;
	node = create_heredocnode(inputstr);
	listappend_heredoc(node, heredoclist);
}


void	heredoc(t_cmd *cmd, int n_heredoc)
{
	char	*inputstr;
	size_t	len;
	int		j;
	t_cmd *tmp_cmdlist;
	t_redir *tmp_redirlist;

	tmp_cmdlist = cmd;
	len = 0;
	if (!n_heredoc)
		return;
	while (tmp_cmdlist)
	{
		tmp_redirlist = tmp_cmdlist->redirlist;
		while (tmp_redirlist)
		{
			while (tmp_redirlist && tmp_redirlist->type != HEREDOC)
				tmp_redirlist = tmp_redirlist->next;
			if(tmp_redirlist)
			{
				while (1)
				{
					//heredoc_prompt(&inputstr);
					printf(">: ");
					getline(&inputstr, &len, stdin);
					j = 0;
					while (inputstr[j] != '\n')
						j++;
					inputstr[j] = '\0';
					//printf("you inserted: %s. Remember the current delimiter is: %s\n", inputstr,tmp_redirlist->delimiter );
					if (strcmp(inputstr,  tmp_redirlist->delimiter) == 0)
						break;
					inputstr[j] = '\n';
					build_heredoclist(inputstr, &(tmp_redirlist->heredoclist));
				}
				tmp_redirlist = tmp_redirlist->next;
			}
		}
		tmp_cmdlist = tmp_cmdlist->next;
	}
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
		data = (t_pipex_data){cmdlist, cmdlist_len, pipematrix, env, envp};
		pipex(&data, exit_code);
	}
	else
	{
		cmdex(cmdlist, env, exit_code);
		printf("comando eseguito\n");
	}
}
