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
		{
			//printf("there is a pipe\n");
			return (THERE_IS_A_PIPE);
		}
	}
	return (THERE_IS_NOT_A_PIPE);
}

// t_command *add_listnode_for_pipe(char **tokenmatrix, current_pipe_index, int i)
// {
// 	int			j;
// 	t_command	*cmdnode;

// 	cmdnode = (t_command *)malloc(sizeof(t_command));
// 	cmd_index = current_pipe_index - i;
// 	commandnode->cmd = strdup(tokenmatrix[cmd_index]); 
// 	commandnode->args = (char **)malloc((n_tokens_fm_cmd_to_pipe + 1) * sizeof(char*));
// 	i = 0;
// 	while ((cmd_index + i) < current_pipe_index)
// 	{
// 		commandnode->args[i] = strdup(tokenmatrix[cmd_index + i]);	
// 		i++;
// 	}
// 	commandnode->args[i] = NULL;
// 	commandnode->prev = NULL;
// 	commandnode->next = NULL;
// 	return (commandnode);
// }

// void commandnode_management_for_pipe(char **tokenmatrix,
// 		int *pipe_index,
// 		int *generictoken_index,
// 		t_command **commandlist)
// {
// 	t_command *commandnode;
// 	commandnode = create_commandnode_for_pipe(tokenmatrix, *pipe_index, *generictoken_index);
// 	listappend_command(commandnode, commandlist);
// }

/*redirectionscheck verifica la presenza di pipe se si restituisce il numero di pipe
possibili implementazioi ulteriori poitrebbe indicare il token, la sua posizione e quanti ce ne sono
al momento è implementato per riconoscere un unico token che si ripete piu volte
*/

// t_redir *redirlist_for_pipe(char **tokenmatrix, int token_index)
// {
// 	t_redir	*redirlist;
// 	int		i;
// 	t_redir	*node;

// 	i = token_index;
// 	redirlist = NULL;
// 	while (tokenmatrix[i] && tokenmatrix[i][0] != PIPE)
// 	{
// 		if (strcmp(tokenmatrix[i], ">") == 0) //estendibile anche alle altre redirection
// 		{
// 			printf("trovato >\n");
// 			node = (t_redir *)malloc(sizeof(t_redir));
// 			//TODO funzione che freea tutti i nodi se c'è un errore ad esempio alla creaizone del terzo
// 			i++;
// 			node->outredir_file = strdup(tokenmatrix[i]); //se fosse stato altra redirezione deve essere implementata
			
// 			node->next = NULL;
// 			listappend_redir(node, &redirlist);
// 		}
// 		i++;
// 	}
// 	return (redirlist);
// }


	//	redirlist = redirections_check(tokenmatrix, pipe_index + current_token);	


/*va a generare una lista di comandi quando ci sono le pipe*/
t_command	*commandlist_for_pipe(char **tokenmatrix)
{
	int			i;
	int			j;
	int			t;
	t_command	*cmdnode;
	t_command	*cmdlist;
	t_redir		*redirnode;

	cmdlist = NULL;

	i = 0;
	while (tokenmatrix[i] != NULL)
	{	
		
		cmdnode = (t_command *)malloc(sizeof(t_command));

		listappend_command(cmdnode, &cmdlist);

		find_last_commandnode(cmdlist)->cmd = tokenmatrix[i]; //TODO verifica se cé bisogno di strdup
		printf("ggg: %s\n", tokenmatrix[i]);
		printf("aaa: %s\n", find_last_commandnode(cmdlist)->cmd);
		i++;
		j = 0;
		while (tokenmatrix[i + j] &&
				strcmp(tokenmatrix[i + j], ">") != 0 &&
				tokenmatrix[i + j][0] != PIPE)
			j++;
		if (j)
		{
			cmdlist->args = (char **)malloc((j + 1) * sizeof(char *));
			t = 0;
			while(t < j)
			{
				cmdlist->args[t] = tokenmatrix[i + t]; //verifica se non e'unn strdup
				t++;
			}
			cmdlist->args[t] = NULL;
		}
		i = i + j;
		while (tokenmatrix[i] &&
				tokenmatrix[i][0] != PIPE)
		{
			if (strcmp(tokenmatrix[i], ">") == 0)
			{
				redirnode = (t_redir *)malloc(sizeof(t_redir));
				listappend_redir(redirnode, &cmdlist->redirlist);
			}
			i++;
			cmdlist->redirlist->outredir_file = tokenmatrix[i];
			i++;
		}
		if (!tokenmatrix[i])
			break;
		i++;
	}
		//test_stampa_args(cmdlist);
		//print_list(cmdlist);
	printf("fff: %s\n", cmdlist->cmd);
	int m =0;
	while (cmdlist)
	{
		printf("r: %s\n",cmdlist->cmd);
		cmdlist = cmdlist->next;
		m++;
	}
	printf("%d\n", m);


	return (cmdlist);
}

