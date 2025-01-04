#include "../minishell.h"



int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix, t_env *genvlist) 
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



void	cmdex(t_command *cmd)
{
	pid_t		pid;
	int			status;
	//t_command	*cmd;

	//cmd = create_cmd(matrix);
	//printf("sono appena entrato nella gestione del singolo comando\n");	
	cmd->path = get_cmdpath(cmd->cmd);
	//printf("ecco il path del comando: %s\n", cmd->path);
	pid = fork(); //forko altrimenti execve mi cancella la shell corrente
	if (pid == CHILD_PID0) //child
		execve(cmd->path, cmd->args, NULL);
	//if (pid > CHILD_PID0) //parent
	waitpid(pid, &status, 0); // Attendi il processo figlio//status e'una variabile che ti consente di verificare come e' terminato il figlio ...il codice di terminazione
	//free_cmd(cmd);

}

void executor(t_command *cmdlist, t_env *global_envlist, int there_is_a_builtin, int there_is_a_pipe)
{
	int	cmdlist_len;
	int	**pipematrix;
	
	cmdlist_len = listlen(cmdlist);
	if (there_is_a_pipe)
	{
		//if (cmdlist_len == 1)
		//	return;
		pipematrix = pipematrix_malloc(cmdlist_len);
		pipex(cmdlist, cmdlist_len, pipematrix, global_envlist);
	}
	else if (cmdlist_len == 1)
	{
		if(!there_is_a_builtin)
			cmdex(cmdlist);
	}
}