#include "../minishell.h"

/*allocates the array of pipes*/
int **pipesalloc(int cmdlist_len)
{
	int	i;
	int	**pipematrix;
	int	num_of_pipes;

	num_of_pipes = cmdlist_len - 1;
	pipematrix = (int **)malloc((num_of_pipes) * sizeof(int *));
	i = 0;
	while (i < num_of_pipes)
	{
		pipematrix[i] = (int *)malloc(2 * sizeof(int));
		i++;
	}
	i = 0;
	while (i < num_of_pipes)
	{
		pipe(pipematrix[i]);
		i++;
	}
	return (pipematrix);
}

/*check if it is at least one pipe*/
int pipecheck(char **tokenmatrix) 
{
	int	i;

	i = 0;
	while (tokenmatrix[i])
	{
		if (tokenmatrix[i][0] != PIPE)
			i++;
		else
			return (THERE_IS_A_PIPE);
	}
	return (THERE_IS_NOT_A_PIPE);
}

void	read_stdin_from_pipe(int **pipematrix, int i)
{
	close(pipematrix[i - 1][WRITE_END]);
	dup2(pipematrix[i - 1][READ_END], STDIN_FILENO);
	close(pipematrix[i - 1][READ_END]);
}

void	write_stdout_in_the_pipe(int **pipematrix, int i)
{
	close(pipematrix[i][READ_END]);
	dup2(pipematrix[i][WRITE_END], STDOUT_FILENO);
	close(pipematrix[i][WRITE_END]);
}

void close_all_pipe(int **pipematrix, int cmdlist_len)
{
	int	n;
	n = 0;
	while (n < (cmdlist_len - 1)) 
	{
		close(pipematrix[n][READ_END]);
		close(pipematrix[n][WRITE_END]);
		n++;
	}
}

void ft_execve(t_command *tmp_cmdlist, t_env *genvlist)
{
	if (builtinex(tmp_cmdlist, &genvlist) == 1)
		return;
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	char **envlist = convert_list_to_matrix(genvlist);
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);
}

void fork_along_pipesloop(int **pipematrix,t_command *tmp_cmdlist, int i, int cmdlist_len, t_env **env)
{
	int pid;
	int saved_stdout;

	pid = fork();
	saved_stdout = dup(STDOUT_FILENO);
	if (pid == 0) 
	{
		if (i > 0) 
		{	
			read_stdin_from_pipe(pipematrix, i);
			if (input_output_routine(tmp_cmdlist->redirlist, saved_stdout) == 0)
				exit(1);
		}
		if (tmp_cmdlist->next) 
		{ 
			write_stdout_in_the_pipe(pipematrix, i);
			if (input_output_routine(tmp_cmdlist->redirlist, saved_stdout) == 0)
				exit(1);
		}
		close_all_pipe(pipematrix, cmdlist_len);
		ft_execve(tmp_cmdlist, *env);
		exit(1);
	}
}


