#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


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
	//tmp = redirlist; //riaggiorno la testa della lista
    return (ret);
}

//TODO: implement the follow for stdin
// t_redir *find_last_type_redirection(t_redir *redirlist, int type_redirection)
// {
//     t_redir *ret;
//     t_redir *tmp;

//     if (!redirlist)
//     	return (NULL);
// 	tmp = redirlist;
//     ret = NULL;
//     while(tmp)
//     {
//         if (tmp->type == type_redirection)
//             ret = tmp;
//         tmp = tmp->next;
//     }
// 	tmp = redirlist; //riaggiorno la testa della lista
//     return (ret);
// }




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

void ft_write_in_latest_outpu_or_append_tredir(t_redir *redirnode)
{
	int fd;
	t_redir	*tmp_redirnode;

	if (!redirnode)
	{
		printf("AAAAAAAAAAAAAAAA: sei in ft_write_in_latest_outpu_or_append_tredir: e il nodo non esiste\n");
		return;
	}
	tmp_redirnode = redirnode;
	if (tmp_redirnode->type == OUTPUT_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (tmp_redirnode->type == APPEND_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
			
void ft_open_all_output_and_append_redirections(t_redir *redirlist)
{
	int fd;
	t_redir	*tmp_redirlist;

	tmp_redirlist = redirlist;
	while(tmp_redirlist)
	{
		if (tmp_redirlist->type == OUTPUT_REDIRECTION) 
		{
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			close(fd);
		}
		if (tmp_redirlist->type == APPEND_REDIRECTION)
		{
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
			close(fd);
		}
		tmp_redirlist = tmp_redirlist->next;
	}
}

void	read_stdin_from_pipe(int **pipematrix, int i)
{
	close(pipematrix[i - 1][WRITE_END]);
	dup2(pipematrix[i - 1][READ_HERE_STDIN], STDIN_FILENO);
	close(pipematrix[i - 1][READ_END]);
}

void	write_stdout_in_the_pipe(int **pipematrix, int i)
{
	close(pipematrix[i][READ_END]);
	dup2(pipematrix[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
	close(pipematrix[i][WRITE_END]);
}

int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) 
{
    pid_t       pid;
    int         i = 0;
    int         m;
    int         fd;
    t_command   *tmp_cmdlist;
    t_redir     *tmp_redirlist;
	int a;
	t_redir	*latest_output_or_append_redir;

    tmp_cmdlist = cmdlist;
    if (tmp_cmdlist)
		printlist(tmp_cmdlist);
	printf("SONO NELLA FUNZIONE: INIZIA IL WHILE\n");
	
	a = 1;
	if (tmp_cmdlist)
		printf("la lista esiste\n\n");
	else
		printf("la lista NON esiste\n");
	int f = 1;
	int b = 1;
	int father = getpid();
	if (tmp_cmdlist)
		printf("sono il padre: %d. Sto per forcare con la tmp_cmdlist ferma su %s\n", father, tmp_cmdlist->cmd);
	else
	{
		printf(" la lista è nulla\n");
		return (0);
	}
	while (tmp_cmdlist) 
	{   
		tmp_redirlist = tmp_cmdlist->redirlist;
        //ls > a < b >> c < d >> e > f | grep u > aa < bb >> cc < dd > ee >> ff
		pid = fork();
        if (pid == CHILD_PID) 
		{
			printf("Sono il figlio %d. Provengo dal fork su: %s\n", pid, tmp_cmdlist->cmd);
			if (i > 0) 
			{// Pipe precedente per input
				read_stdin_from_pipe(pipematrix, i);
			}
			if (tmp_cmdlist->next) 
			{ 
				if (tmp_cmdlist->redirlist)
				{
					ft_open_all_output_and_append_redirections(tmp_cmdlist->redirlist);
					latest_output_or_append_redir = find_latest_redirection_among_outpus_and_appends(tmp_cmdlist->redirlist);
					ft_write_in_latest_outpu_or_append_tredir(latest_output_or_append_redir);
				}
				else
				{
					write_stdout_in_the_pipe(pipematrix, i);
				}
			} 
			else
			{
				if (tmp_cmdlist->redirlist)
				{
					ft_open_all_output_and_append_redirections(tmp_cmdlist->redirlist);
					latest_output_or_append_redir = find_latest_redirection_among_outpus_and_appends(tmp_cmdlist->redirlist);
					ft_write_in_latest_outpu_or_append_tredir(latest_output_or_append_redir);
				}
			}
			tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
			execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
			perror("execve fallita");
			//exit(1);
		} 
		else if (pid > 0) 
		{
			printf("\nSono nel figlio: %d. Cmd attuale: %s\n", pid, tmp_cmdlist->cmd);
			// Processo padre man mano che si avanza chiude le pipe precedentamente aperte.
            if (i > 0) 
			{
                close(pipematrix[i - 1][READ_END]);
                close(pipematrix[i - 1][WRITE_END]);
            }
			tmp_cmdlist = tmp_cmdlist->next;
			if (tmp_cmdlist)
				printf("padre: %d, cmd successivo: %s\n", pid, tmp_cmdlist->cmd);
			else
				printf("AAA il comando successivo è nullo. Quindi non si entrerà nel while. Significa che non si entrerà piu nel while che cicla lungo i comandi\n");
            i++;
			printf("sto abbandonando il figlio  %d\n\n", pid);
        } 
		else 
		{
            perror("Errore fork");
            exit(1);
        }
    }
    while ((pid = wait(NULL)) > 0) 
	{
        printf("Figlio n. %d: terminato (exit-code: %d)\n", f, pid);
		f++;
    }
    return 0;
}
