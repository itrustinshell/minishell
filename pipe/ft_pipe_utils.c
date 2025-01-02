#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/*n questa funzione viene utilizzata un altra funzione che è pipe(). 
  Questa genera un canale di comunicazione tra due processi (processo padre e processo figlio. 
  Questo canale di comunicazione ha due endpoint: uno per la lettura [0], e uno per la scrittura [1]. 
  pipe() restituisce un array di due int. Questi due int sono i file descriptor di questi due endpoint.
  Ora nella funzione che segue è come se io volessi avere un elenco di tuti questi canali presenti. 
  Ad esempio se esistono 3 pipe nella mia stringa di inpt allora voglio che ci siano 3 pipe(), 
  ovvero 3 canali di comunicazione, ovvero 3 array di 2 int. 
  Questo elenco di questi array pertanto è un array di array. Il primoelemento è il primo array di due int, 
  il secondo elemento è il secondo array di due int (la seconda pipe()), il terzo elemento è il terzo array 
  di due int (la terza pipe della mia stringa).
  Cosi facendo io posso a ceddere a questi array di due int attraverso un array più grande che li racchiude.
Ora, se vediamo bene, il numero di pipe è sempre uguale al numero dei comandi meno uno.Se hai due comandi 
infatti avrai appunto una pipe, ovvero 2 - 1 = 1.
  */
int **pipematrix_malloc(int cmdlist_len)
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

int check_pipe_symbol(char **tokenmatrix)
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

t_command	*commandlist_for_pipe(char **tokenmatrix, char **envp)
{
	int			i;
	int			j;
	int			t;
	t_command	*cmdnode;
	t_command	*cmdlist;
	t_redir		*redirnode;

	redirnode = NULL;
	cmdlist = NULL;
	i = 0;
	while (tokenmatrix[i] != NULL)
	{	
		cmdnode = (t_command *)malloc(sizeof(t_command));
		cmdnode->redirlist = NULL;
		cmdnode->envp = envp;
		listappend_command(cmdnode, &cmdlist);
		last_cmdnode(cmdlist)->cmd = tokenmatrix[i]; //TODO verifica se cé bisogno di strdup
		if (strcmp(cmdnode->cmd, "echo") == 0)
		{
			cmdnode->builtin = 1;
		}
		j = 0;
		while (tokenmatrix[i + j] && strcmp(tokenmatrix[i + j], ">") != 0 && strcmp(tokenmatrix[i + j], "<") != 0 && strcmp(tokenmatrix[i + j], ">>") != 0 && tokenmatrix[i + j][0] != PIPE)
			j++;
		if (j)
		{
			last_cmdnode(cmdlist)->args = (char **)malloc((j + 1) * sizeof(char *));
			t = 0;
			while(t < j)
			{
				last_cmdnode(cmdlist)->args[t] = tokenmatrix[i + t]; //verifica se non e'unn strdup
				t++;
			}
			last_cmdnode(cmdlist)->args[t] = NULL;
		}
		i = i + j;
		while (tokenmatrix[i] && tokenmatrix[i][0] != PIPE)
		{
			if (strcmp(tokenmatrix[i], ">") == 0 || strcmp(tokenmatrix[i], "<") == 0 || strcmp(tokenmatrix[i], ">>") == 0)
			{
				redirnode = (t_redir *)malloc(sizeof(t_redir));
				
				if ((strcmp(tokenmatrix[i], ">") == 0))
					redirnode->type = OUTPUT_REDIRECTION;
				if ((strcmp(tokenmatrix[i], "<") == 0))
					redirnode->type = INPUT_REDIRECTION;
				if ((strcmp(tokenmatrix[i], ">>") == 0))
					redirnode->type = APPEND_REDIRECTION;
				i++;
				redirnode->outredir_file = tokenmatrix[i];
				listappend_redir(redirnode, &last_cmdnode(cmdlist)->redirlist);
			}
			i++;
		}
		if (!tokenmatrix[i])
			break;
		i++;
	}
	return (cmdlist);
}

