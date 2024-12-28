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
	//tmp = redirlist; //riaggiorno la testa della lista
    return (ret);
}

void ft_write_in_latest_outpu_or_append_redir(t_redir *redirnode)
{
	int		fd;
	t_redir	*tmp_redirnode;

	if (!redirnode)
		return;
		//printf("AAAAAAAAAAAAAAAA: sei in ft_write_in_latest_outpu_or_append_tredir: e il nodo non esiste\n");
		

	tmp_redirnode = redirnode;
	if (tmp_redirnode->type == OUTPUT_REDIRECTION)
	{
		//printf("ATTENZIONE!!!!!!! STO PER REDIRIGERE L'OUTPUT REDIR\n");
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
	else
		printf("beccato!!!! coa sta succedendo?\n");
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


//AAA ls < a > b | ls ...se non esisste input si ferma e non da neanche output...ma va al successivo comado e lo esegue

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

void	get_input_from_latest_input_redir(t_redir *latest_input_redir)
{
	int	fd;

	printf("ecco latest input: %s\n", latest_input_redir->outredir_file);
	fd = open(latest_input_redir->outredir_file, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
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
	t_redir	*latest_input_redir;

    
    // if (tmp_cmdlist)
	// 	printlist(tmp_cmdlist);
	// printf("SONO NELLA FUNZIONE: INIZIA IL WHILE\n");
	
	a = 1;
	// if (tmp_cmdlist)
	// 	printf("la lista esiste\n\n");
	// else
	// 	printf("la lista NON esiste\n");
	int f = 1;
	int b = 1;
	int father = getpid();
	// if (tmp_cmdlist)
	// 	printf("sono il padre: %d. Sto per forcare con la tmp_cmdlist ferma su %s\n", father, tmp_cmdlist->cmd);
	tmp_cmdlist = cmdlist;
	if (!tmp_cmdlist)
	{
		printf(" la lista è nulla\n");
		return (0);
	}
	while (tmp_cmdlist) 
	{   
		tmp_redirlist = tmp_cmdlist->redirlist;
		pid = fork();
        if (pid == CHILD_PID) 
		{
			//printf("Sono il figlio %d. Provengo dal fork su: %s\n", pid, tmp_cmdlist->cmd);
			if (i > 0) 
			{// Pipe precedente per input
			//	printf("sonon entrato nella gestione della lettura_leggerò dalla pipe\n");
				
				read_stdin_from_pipe(pipematrix, i);
			}
			if (tmp_cmdlist->next) 
			{ 
				if (tmp_cmdlist->redirlist)
				{
					latest_input_redir = find_latest_input_redirection(tmp_cmdlist->redirlist);
					if (latest_input_redir)
					{
					//	printf("esiste uninput\n");
						if (access(latest_input_redir->outredir_file, F_OK) == 0)
							get_input_from_latest_input_redir(latest_input_redir);
						else
						{
							printf("INSERIRE IL MESSAGGIO DI ERRORE!!!!!\n");
							return(1);
						}
					}
					ft_open_all_output_and_append_redirections(tmp_cmdlist->redirlist);
					latest_output_or_append_redir = find_latest_redirection_among_outpus_and_appends(tmp_cmdlist->redirlist);
					//printf("MA QUESTO LO LEGGI ??? %s\n", latest_output_or_append_redir->outredir_file);
					ft_write_in_latest_outpu_or_append_redir(latest_output_or_append_redir);
				}
				else
					write_stdout_in_the_pipe(pipematrix, i);
				//AAA sto notanchdo che forse quando salto la prima volta uesto...salto cmq il passaggio di chiusura dells pipe di lettura
				//forse non è un problema....ma verifica
			} 
			else
			{
				//printf("verifico se ci sono redirections\n");
				if (tmp_cmdlist->redirlist)
				{
					ft_open_all_output_and_append_redirections(tmp_cmdlist->redirlist);
					latest_output_or_append_redir = find_latest_redirection_among_outpus_and_appends(tmp_cmdlist->redirlist);
					ft_write_in_latest_outpu_or_append_redir(latest_output_or_append_redir);
				}
			}
			tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
			//printf("%d: eseguo\n", i);
			execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
			perror("execve fallita");
			//exit(1);
		} 
		else if (pid > 0) 
		{
			//printf("\nSono nel figlio: %d. Cmd attuale: %s\n", pid, tmp_cmdlist->cmd);
			// Processo padre man mano che si avanza chiude le pipe precedentamente aperte.
            if (i > 0) 
			{
                close(pipematrix[i - 1][READ_END]);
                close(pipematrix[i - 1][WRITE_END]);
            }
			tmp_cmdlist = tmp_cmdlist->next;
			// if (tmp_cmdlist)
			// 	printf("padre: %d, cmd successivo: %s\n", pid, tmp_cmdlist->cmd);
			// else
			// 	printf("AAA il comando successivo è nullo. Quindi non si entrerà nel while. Significa che non si entrerà piu nel while che cicla lungo i comandi\n");
            i++;
			//printf("sto abbandonando il figlio  %d\n\n", pid);
        } 
		else 
		{
            perror("Errore fork");
            exit(1);
        }
    }
    while ((pid = wait(NULL)) > 0) 
	{
      //  printf("Figlio n. %d: terminato (exit-code: %d)\n", f, pid);
		f++;
    }
    return 0;
}
