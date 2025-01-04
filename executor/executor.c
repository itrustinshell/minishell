#include "../minishell.h"

int execute_pipe(t_command *cmdlist, int cmdlist_len, int **pipematrix, t_env *genvlist) 
{
	pid_t pid;
	int i;
	
	t_command *tmp_cmdlist = cmdlist;

	if (!tmp_cmdlist) 
		return (0);
	i = -1;
	while (++i < cmdlist_len) 
	{
		if (i > 0) 
			tmp_cmdlist = tmp_cmdlist->next;
		fork_along_pipesloop(pipematrix, tmp_cmdlist, i, cmdlist_len, genvlist);
	}
	close_all_pipe(pipematrix, cmdlist_len);
	while ((pid = wait(NULL)) > 0)
	{
		//printf("processo terminato\n");
	}
	return (0);
}

void execute_multiple_cmd(t_command *cmdlist, t_env *global_envlist)
{
	int	cmdlist_len;
	int	**pipematrix;

	cmdlist_len = listlen(cmdlist);
	pipematrix = pipematrix_malloc(cmdlist_len);
	execute_pipe(cmdlist, cmdlist_len, pipematrix, global_envlist);
}

void ft_execve(t_command *tmp_cmdlist, t_env *genvlist)
{
	if (check_builtin_in_cmdlist(tmp_cmdlist, genvlist) == 1)
		return;
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	char **envlist = convert_list_to_matrix(genvlist);
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);
}

void	execute_single_cmd(char **matrix)
{
	pid_t		pid;
	int			status;
	t_command	*cmd;

	cmd = create_cmd(matrix);
	//printf("sono appena entrato nella gestione del singolo comando\n");	
	cmd->path = get_cmdpath(cmd->cmd);
	//printf("ecco il path del comando: %s\n", cmd->path);
	pid = fork(); //forko altrimenti execve mi cancella la shell corrente
	if (pid == CHILD_PID0) //child
		execve(cmd->path, cmd->args, NULL);
	//if (pid > CHILD_PID0) //parent
	waitpid(pid, &status, 0); // Attendi il processo figlio//status e'una variabile che ti consente di verificare come e' terminato il figlio ...il codice di terminazione
	free_cmd(cmd);

}

