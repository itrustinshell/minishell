#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


void printlist(t_command *cmdlist)
{
	t_command	*tmp_cmdlist;
	t_redir		*tmp_redirlist;
	int			m;

	printf("\nDEBUG: STO STAMPANDO LA LISTA DI COMANDI CON ARGOMENTI E REDIRECTION\n");
	m = 0;
	tmp_cmdlist = cmdlist;
	while (tmp_cmdlist)
	{
		printf("ecco il comando: %s\n", tmp_cmdlist->cmd);
		if (tmp_cmdlist->args)
		{
			printf("ecco gli argomenti di %s:\n", tmp_cmdlist->cmd);
			m = -1;
			while(tmp_cmdlist->args[++m])
				printf("args: %s\n", tmp_cmdlist->args[m]);
		}
		if (tmp_cmdlist->redirlist)
		{
			printf("ecco le redirections di %s:\n", tmp_cmdlist->cmd);
			tmp_redirlist = tmp_cmdlist->redirlist;
			while (tmp_redirlist)
			{
				printf("redir: %s\n", tmp_redirlist->outredir_file);
				tmp_redirlist = tmp_redirlist->next;
			}
		}
		else
			printf("non ci sno argomenti\n");
		tmp_cmdlist = tmp_cmdlist->next;
	}
	printf("DEBUG: HO TERMINATO DI STAMPARE LISTA, ARGOMENTI E RELATIVE REDIRECTIONS\n\n");
}

t_redir *find_latest_redirection_among_outpus_and_appends(t_redir *redirlist)
{
	t_redir *ret;
	t_redir *tmp;

	if (!redirlist)
		return (NULL);
	tmp = redirlist;
	ret = NULL;
	while(tmp)
	{
		if (tmp->type == OUTPUT_REDIRECTION || tmp->type == APPEND_REDIRECTION)
			ret = tmp;
		tmp = tmp->next;
	}
	return (ret);
}

void ft_write_in_latest_outpu_or_append_redir(t_redir *redirnode)
{
	int		fd;
	t_redir	*tmp_redirnode;

	if (!redirnode)
		return;
	tmp_redirnode = redirnode;
	if (tmp_redirnode->type == OUTPUT_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (tmp_redirnode->type == APPEND_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
			
int ft_open_all_output_and_append_redirections(t_redir *redirlist)
{
	int fd;
	int	output_append_exists;
	t_redir	*tmp_redirlist;

	output_append_exists = 0;
	tmp_redirlist = redirlist;
	while(tmp_redirlist)
	{
		if (tmp_redirlist->type == OUTPUT_REDIRECTION) 
		{
			output_append_exists = 1;
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			close(fd);
		}
		if (tmp_redirlist->type == APPEND_REDIRECTION)
		{
			output_append_exists = 1;
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
			close(fd);
		}
		tmp_redirlist = tmp_redirlist->next;
	}
	return (output_append_exists);
}

void output_append_management(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist)
		return;
	if (ft_open_all_output_and_append_redirections(redirlist) == 0)
		return;
	redirnode = find_latest_redirection_among_outpus_and_appends(redirlist);
	if (!redirnode)
		return;
	ft_write_in_latest_outpu_or_append_redir(redirnode);
}

t_redir *find_latest_input_redirection(t_redir *redirlist)
{
	t_redir	*tmp_redirlist;
	t_redir	*ret;

	if (!redirlist)
		return (NULL);
	ret = NULL;
	tmp_redirlist = redirlist;
	while(tmp_redirlist)
	{
		if (tmp_redirlist->type == INPUT_REDIRECTION)
			ret = tmp_redirlist;
		tmp_redirlist = tmp_redirlist->next;
	}
	return (ret);
}

int	get_input_from_latest_input_redir(t_redir *redirlist, int saved_stdout)
{
	int	fd;
	t_redir	*latest_input_redir;
	int ret;

	ret = 1;
	if (!redirlist)
		return (ret);
	latest_input_redir = find_latest_input_redirection(redirlist);
	if (!latest_input_redir)
		return (ret);
	if (access(latest_input_redir->outredir_file, F_OK) == 0)
	{
		fd = open(latest_input_redir->outredir_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		dup2(saved_stdout, STDOUT_FILENO);
		printf("ERROREEEEEEEEE\n");
		ret = 0;
	}
	return (ret);
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

int input_output_routine(t_redir *redirlist,  int saved_stdout)
{
	if (get_input_from_latest_input_redir(redirlist, saved_stdout) == 0)
		return(0);
	output_append_management(redirlist);
	return (1);
}


int ft_echo(int argc, char **argv)
{
	int i;

	if (strcmp(argv[1], "-n") == 0)
	{
		i = 2;
		while (i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("%s", argv[i]);
	}
	else
	{
		i = 1;
		while (i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("%s\n", argv[i]);
	}
	return(1);
}

int ft_pwd()
{
	char	*cwd_buffer;

	cwd_buffer = NULL;
	cwd_buffer = getcwd(NULL, 0);
	printf("%s\n", cwd_buffer);



	free(cwd_buffer);
	return (1);
}

int ft_cd(char **argv)
{
	char	*cwdpath;
	char	*strmaps;
	char	**mapindications;
	char	**splittedcwd;
	int		i;
	char	*finalpath;

	strmaps = argv[1];
	cwdpath = getcwd(NULL, 0);
	printf("hei I am in cd funciton and this is the current path:\n%s\n", cwdpath);
	if (strmaps)
	{
		printf("yes strmpas exists\n");
	}
	else
	{
		printf("no, strmaps doesn't exist\n");
		return 0;
	}
	chdir(strmaps);
	char *str;
	str = getcwd(NULL, 0);
	printf("this is new dir:%s\n", str);
	//a questo punto posso dire che se lo strcmp tra str e cwdpath non cambia significa che non ho cambiato cartella e quindi che il path non esiste e bash allora puo generare un messaggo di errore.
	return (1);
}


void ft_exit()
{
	exit(1);
}

int check_builtin(t_command *tmp_cmdlist, int saved_stdout, t_env *genvlist)
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

void ft_execve(t_command *tmp_cmdlist, int saved_stdout, t_env *genvlist)
{
	if (check_builtin(tmp_cmdlist, saved_stdout, genvlist) == 1)
		return;
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
}

void fork_along_pipesloop(int **pipematrix,t_command *tmp_cmdlist, int i, int cmdlist_len, t_env *genvlist)
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
		ft_execve(tmp_cmdlist, saved_stdout, genvlist);
		exit(1);
	}
}

int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix, t_env *genvlist) 
{
	pid_t pid;
	int i;
	int f = 1;
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
