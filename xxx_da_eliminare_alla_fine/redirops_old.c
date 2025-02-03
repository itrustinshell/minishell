#include "../minishell.h"

/*operations with output and append redirections*/
void	oa_redirops(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist)
		return ;
	if (oa_rediropen(redirlist) == 0)
		return ;
	redirnode = oa_redirlast(redirlist);
	if (!redirnode)
		return ;
	oa_redirwrite(redirnode);
}

/*	check If exist an input redirection or an heredoc */
int	ih_redirops(t_redir *redirlist, int saved_stdout)
{
	int		fd;
	t_redir	*latest_input_redir;
	int		ret;

	ret = 1;
	if (!redirlist) //if there is no redirlist nothing change
		return (ret);
	latest_input_redir = ih_redirlast(redirlist); //vado a prendermi l'ultima input redirection
	if (!latest_input_redir)
		return (2);
	if (latest_input_redir->type == INPUT_REDIRECTION)
	{
		/*se l'ultima ih_redirection è un input redirection
		allora vado a controllare se esiste ilfile da cui dovrò
		leggere l'input. Se esiste lo apro con open e uso il fd per 
		redigirege lo STDINPUT_ Se invece il file non esiste allora
		ritorno zero: Questo sarà un errore che terminerà l'executor*/
		ret = 3;
		if (access(latest_input_redir->file, F_OK) == 0)
		{
			fd = open(latest_input_redir->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
		{	
			dup2(saved_stdout, STDOUT_FILENO);
			printf("ERROREEEEEEEEE\n");
			ret = 0;
		}
	}
	else if(latest_input_redir->type == HEREDOC)
	{
		/*se invece l'ultimo ih è un heredoc
		1) mi  creo una pipe. Infatti userò questa pipe per scrivere dentro
		i vari input che l'utente inserirà durante l'esecuzione degli heredoc.
		Qunindi poi sara' da questa pipe che verrà letto appunto l'input.*/
		int fdpipe[2];
		pipe(fdpipe); //inizializzo la pipe che ora ha due FD, uno di lettura e uno per scrittura
        if (latest_input_redir->heredoclist)
		{
			/* se esiste una lista di heredoc (ovvero gli input inseriti dall'utente con l'heredoc)
				vado a scorrere lungo questa lista. PEr semplicità la assegno a un t_heredo *current-
				Quindi man mano che scorro vado a scrivere nella pipe (estremità di scrittura),
				l'input che aveva inserito l'utente 
				In questo modo avrò alla fine la mia pipe con tutto l'input dell'utente.
				Ptro' quindi dopo redirigere lo stdin a questa pipe
			*/
            t_heredoc *current = latest_input_redir->heredoclist;
            while (current) 
			{
                write(fdpipe[1], current->input, strlen(current->input));
                current = current->next;
            }
            close(fdpipe[1]); // Chiudi il lato di scrittura della pipe
        }
        dup2(fdpipe[0], STDIN_FILENO);
        close(fdpipe[0]); // Chiudi il lato di lettura della pipe
		return (4);
	}
	return (ret);
}

/*	the whole input-heredoc-output-append (ihoa) operations
	i_ret = 0 -> The symbol of iredi was found...but the file doesnt exist.
	i_ret = 2 -> NO iredr o heredoc found!!!
	i_ret = 3 -> last inputredir is an iredir
	i_ret = 4 -> last inputredir is an HEREDOC	*/
int	ihoa_redirops(t_redir *redirlist, int saved_stdout)
{
	int	ih_ret;

	ih_ret = ih_redirops(redirlist, saved_stdout);
	if (ih_ret == 0)
		return (0);
	oa_redirops(redirlist);
	return (ih_ret);
}
