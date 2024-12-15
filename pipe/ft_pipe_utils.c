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
int **create_pipesarray(int cmdlist_len)
{
	int	i;
	int	**pipesarray;
	int	num_of_pipes;

	num_of_pipes = cmdlist_len - 1;
	pipesarray = (int **)malloc((num_of_pipes) * sizeof(int *));
	i = 0;
	while (i < num_of_pipes)
	{
		pipesarray[i] = (int *)malloc(2 * sizeof(int));
		i++;
	}
	i = 0;
	while (i < num_of_pipes)
	{
		pipe(pipesarray[i]);
		i++;
	}
	return (pipesarray);
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
			printf("there is a pipe\n");
			return (THERE_IS_A_PIPE);
		}
	}
	return (THERE_IS_NOT_A_PIPE);
}

t_command *create_commandnode_for_pipe(char **tokenmatrix, int current_pipe_index, int n_tokens_fm_cmd_to_pipe)
{
	int i;
	int cmd_index;
	t_command *commandnode;

	commandnode = (t_command *)malloc(sizeof(t_command));
	cmd_index = current_pipe_index - n_tokens_fm_cmd_to_pipe;
	commandnode -> cmd = strdup(tokenmatrix[cmd_index]); 
	commandnode -> args = (char **)malloc((n_tokens_fm_cmd_to_pipe + 1) * sizeof(char*));
	i = 0;
	while ((cmd_index + i) < current_pipe_index)
	{
		commandnode -> args[i] = strdup(tokenmatrix[cmd_index + i]);	
		i++;
	}
	commandnode->args[i] = NULL;
	commandnode->prev = NULL;
	commandnode->next = NULL;
	return (commandnode);
}


/*becouse of norm i got this function
the function is void so it modify something inside that reflects in the code (that is the list).
Thi function create a new command node.
Then the function append this newcreated commandnode to the list.
It updates the pipe_index passed byreference, to allow the next iteration to start from there.
 */
void commandnode_management_for_pipe(char **tokenmatrix, int *pipe_index, int *generictoken_index, t_command **commandlist)
{
	t_command *commandnode;
	//pipex_index è sempre uguale a se stesso più il generictoken_index. Perchè ? allora inzialmente sono entrami zero. Aumenta slo il generic_token_index. Quando viene trovato un pipe allora il generic token è su quel pipe. Ecco queindi che per aggiornare il pipeindex si fa pipexindex + zero; Ora pipeindex punta appunto al pipe (è infandi l'indiec del pipe)...per le iterazioni successive pipe_index + generic_index inizia da qui....e ovviamente generic_index sarà reimpostato a zero. In questo modo se viene incontrata una nuova pipe....allora si farà dinuovo valore della pipe veccia piu valore corrente di generic_token
	commandnode = create_commandnode_for_pipe(tokenmatrix, *pipe_index, *generictoken_index);
	append_to_commandlist(commandnode, commandlist);
}

/*va a generare una lista di comandi quando ci sono le pipe*/
t_command *commandlist_for_pipe(char **tokenmatrix)
{
	int			pipe_index;
	int			generictoken_index;
	t_command	*commandlist;

	commandlist = NULL;
	pipe_index = -1;
	generictoken_index = 0;
	while (tokenmatrix[++pipe_index + (generictoken_index)] != NULL)
	{
		while (tokenmatrix[pipe_index + (generictoken_index)] && tokenmatrix[pipe_index + generictoken_index][0] != PIPE)
			generictoken_index++; //auementa lungo la matrice di token fino a che non incontri pipe
		//se è stata incontrata una pipe allora si procede al commandnode_managment che genra un nodo per un comando e lo inserisce nella lista
		if (tokenmatrix[pipe_index + generictoken_index] && tokenmatrix[pipe_index + generictoken_index][0] == PIPE)
		{
				pipe_index = pipe_index + generictoken_index;
			commandnode_management_for_pipe(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			generictoken_index = 0;
			continue;
		}
		else
		{
			/*qui entri sostanzialmente quando sei arrivato alla fine della matrice: non ci osno piu pipe ma il null. 
			A questo punto esegui il command managemente per la creazione e l'append della parte a destra della pipe
			infatti fino ad adesso man mano che proseguivi riempivi i comandi e argomenti a sinistra dlela pipe...
			ora è arrivato il turno dell'ultimo. La logica non cambia e anche se sei sul NULL, il programma eseguirà tutte 
			le  operazioni con il pipe_index proprio sul null.
			 */	
			
			pipe_index = pipe_index + generictoken_index;
			commandnode_management_for_pipe(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			break;
		}
	}
//	test_stampa_args(commandlist);
	
	print_list(commandlist);
	return commandlist;
}

