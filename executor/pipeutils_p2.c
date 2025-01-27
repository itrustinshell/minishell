#include "../minishell.h"

/*exec builtins or external cmd*/
void	ft_execve(t_cmd *tmp_cmdlist, t_env *genvlist, int *exit_code)
{
	char	**envlist;
	//if the execution of a builtin was succesfully done that's all.
	if (builtinex(tmp_cmdlist, &genvlist, exit_code) == 1)
		return ;
	//There was no built-in, so i go to find the path of the external cmd
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	//i trasform the list of env in char **env, iot to pass them to execve param.
	envlist = litoma(genvlist);
	//i call execve. Previously in the code I made operations with redirection
	//it means that when i execute execve, it knows who is its input and 
	//where output (these are not info to pass via args_ execve knows how to do it
	// so it applies your changes in redirections stream.)
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);
}

/*fork along the pipes*/
void	pipefork(int **pipematrix, t_cmd *cmdlist, 
		int i, int cmdlist_len, t_env **env, int *exit_code)
{
	int	pid;
	int	saved_stdout;
	int	n_heredoc;
	int	ret;
	t_cmd *tmp_cmdlist;

	tmp_cmdlist = cmdlist;
	pid = fork(); //la prima cosa che fa questa funzinoa quando è chiamata è forkare
	saved_stdout = dup(STDOUT_FILENO); //mi salvo lo stdout perchè dopo lo ripristinerò
	if (pid == 0) //ora sono nel figlio_ In pratica questa funzione esegue il codice del figlio.
	{
		n_heredoc = 0;
		/*qui dentro ci entro a partire dal secondo comando in poi perchè
			è dal secondo comando in poi che l'output del precedente comando
			diventa un input_ I primo comando non riceve input che siano output di precedenti
			comandi.*/
		if (i > 0) 
		{
			//con piperead predisponiamo la pipe per andare a prendere l'input dalla pipe.
			//Nella pipe troveremo l'output scritto dal precedente comando.
			//questo output sarà cosi l'input del comando che ora lo sta leggendo.
			piperead(pipematrix, i);
			/*Ora una volta predisposte le pipe, io devo vedere se ci sono eventuali
			rerirection che mi vanno a modificare qualcosa_ Ad esempio potrebbero esserci
			delle input redirection. Se infatti ci fossero delle input redirection allora 
			io non dovrò più andare a leggere dalla pipe. Questo significa che andrò a 
			ricambiare lo standard input facendolo corrispondere ad esempio al filde_descriptor
			di un file che segue l'input redirection...Stesse considerazioni valgono 
			per outpu, append redirection. ioa reirops infatti sta per "input output append redirection operation"*/
			ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
			/*se ioa_redirops ritorna 0 significa che c'è stato un errore.
				in particolare c'era un input redirection, ma quest'ultima aveva come
				suo riferimento per l'input, un file inesistente.
			*/
			if (ret == 0) 
				exit(1);
		}
		if (tmp_cmdlist->next)
		{
			pipewrite(pipematrix, i);
			ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
			printf("sono stati rilevati %d heredoc\n", n_heredoc);
			if (ret == 0)
				exit(1);			
		}
		pipeclose(pipematrix, cmdlist_len);
		ft_execve(tmp_cmdlist, *env, exit_code);
		exit(1);
	}
}
