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
	//else
		//printf("beccato!!!! coa sta succedendo?\n");
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

int	get_input_from_latest_input_redir(t_redir *redirlist)
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
		//printf("ecco latest input: %s\n", latest_input_redir->outredir_file);
	if (access(latest_input_redir->outredir_file, F_OK) == 0)
	{
		fd = open(latest_input_redir->outredir_file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
		ret = 0;
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
/*
qui mi arriva la comand list e la pipematrix
ho gia tutte le pipe.



 */


void fork_along_pipesloop(int **pipematrix,t_command *tmp_cmdlist, int i, int cmdlist_len)
{
		int n;
        int pid = fork();
        int saved_stdout;

		saved_stdout = dup(STDOUT_FILENO);

        if (pid == 0) 
		{  // Processo figlio
          //  printf("child %d: sono nel comando %s\n", i, tmp_cmdlist->cmd);
			if (i > 0) 
			{  // Se non è il primo comando, leggi dalla pipe precedente

             //   printf(GREEN"leggerò dalla pipe\n"RESET);
				read_stdin_from_pipe(pipematrix, i);
				if (get_input_from_latest_input_redir(tmp_cmdlist->redirlist) == 0)
				{
					dup2(saved_stdout, STDOUT_FILENO);
					printf("ERROREEEEEEEEE\n");
					exit(1);
				}
				output_append_management(tmp_cmdlist->redirlist);



            }
            if (tmp_cmdlist->next) 
			{  // Se non è l'ultimo comando, scrivi nella pipe successiva
             //   printf(GREEN"sto per scrivere nella pipe\n"RESET);
				write_stdout_in_the_pipe(pipematrix, i);
				if (get_input_from_latest_input_redir(tmp_cmdlist->redirlist) == 0)
				{
					dup2(saved_stdout, STDOUT_FILENO);
					printf("ERROREEEEEEEEE\n");
					exit(1);
				}
				output_append_management(tmp_cmdlist->redirlist);


            }
            // Chiudi tutte le pipe inutilizzate
			close_all_pipe(pipematrix, cmdlist_len);


            tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
         //   printf("%d: eseguo\n", i);
         //   printf(MAGENTA"sto eseguendo il figlio  %d\n\n"RESET, i);
            execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
           // perror("execve fallita");
            exit(1);  // Se execve fallisce
        }
}


int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) 
{
    pid_t pid;
    int i;
    int f = 1;
    t_command *tmp_cmdlist = cmdlist;

    if (!tmp_cmdlist) 
    {
       // printf("la lista è nulla\n");
        return (0);
    }
   // printlist(tmp_cmdlist);
    i = -1;
   // printf("lunghezza cmdlist: %d\n", cmdlist_len);

    // Ciclo per eseguire tutti i comandi della pipeline
	 while (++i < cmdlist_len) 
    {
       // printf("i: %d\n", i);
        if (i > 0) 
		{
            tmp_cmdlist = tmp_cmdlist->next;
            //if (tmp_cmdlist)
            //    printf("figlio: %d, cmd successivo: %s\n", i, tmp_cmdlist->cmd);
           // else
             //   printf(RED"AAA: il comando successivo è nullo. Non si cicla più lungo i comandi\n"RESET);
        }
      //  printf("parent: sono nel comando %s\n", tmp_cmdlist->cmd);
		fork_along_pipesloop(pipematrix, tmp_cmdlist, i, cmdlist_len);
	}
    // Processo padre: man mano che si avanza chiude le pipe precedentemente aperte
   // printf(MAGENTA"\nAAAAAAA Sono nel padre:\n"RESET);
	close_all_pipe(pipematrix, cmdlist_len);

    // Il padre aspetta la terminazione di tutti i figli
    while ((pid = wait(NULL)) > 0) {
        // printf("Figlio n. %d: terminato (exit-code: %d)\n", f, pid);
        f++;
    }
    return (0);
}


