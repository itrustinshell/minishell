#include "minishell.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/wait.h>
#include <sys/types.h>

int check_redirection_symbol(char **tokenmatrix)
{
	int	i;

	i = 0;
	while (tokenmatrix[i])
	{
		if (tokenmatrix[i][0] != OUTPUT_REDIRECTION)
			i++;
		else
		{
			//printf("there is a pipe\n");
			return (THERE_IS_A_REDIRECTION);

		}
	}
	return (THERE_IS_NOT_A_REDIRECTION);
}

void cmd_before_single_outredirection(char **matrix)
{
	t_command	*cmd;
	int			i;
	int			fd;

	cmd = (t_command *)malloc(sizeof(t_command));
	
	cmd->cmd = strdup(matrix[0]);
	cmd->path = get_cmdpath(cmd->cmd);
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
		i++;
	cmd->args = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
	{
		cmd->args[i] = strdup(matrix[i]);
		i++;
	}
	cmd->args[i] = NULL;
	i++;
	fd = open(matrix[i], O_WRONLY | O_CREAT | O_TRUNC, 0466); //TODO aprire il file dato in path assoluto e relativo
	
	int pid = fork();

	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execve(cmd->path, cmd->args, NULL);
	}
	else
	{
		close(fd);
		wait(NULL);
	}		
}




int main(int argc, char **argv, char **envp)
{
	char *inputstr;
	char *cwdpath;
	size_t len;
	char **matrix;
	int i;
	t_command *cmd;
	t_command	*cmdlist;
	int			cmdlist_len;
	int			**pipematrix;
	char *buffer = NULL;
	
	 char **l_envp;
	 char **g_envp;
	
	l_envp = NULL;
	g_envp = NULL;

	t_env	*genvlist;	//le variabili di ambiente glbale in lista
	t_env	*lenvlist;	//le variabili di ambiente locale in lista
	t_env	*lenvnode;
	genvlist = NULL;	
	lenvlist = NULL; 	
	genvlist = copy_envp(envp);
	while (1)
	{	
		len = 0;
		inputstr = NULL;
		cwdpath = getcwd(NULL, 0);
		printf("%s💪💪💪: ", cwdpath);
		getline(&inputstr, &len, stdin); //free di inputstr fatto!
		matrix = tokenizer(inputstr);
		if (check_pipe_symbol(matrix) == THERE_IS_A_PIPE)
		{
			cmdlist = commandlist_for_pipe(matrix, g_envp); //se c'è almeno una pipe, viene costruita una lista di comandi
			cmdlist_len = listlen(cmdlist);
			pipematrix = pipematrix_malloc(cmdlist_len);
			pipex(cmdlist, cmdlist_len, pipematrix, genvlist);
		}
		else if (strcmp(matrix[0], "add") == 0)//poi aggiungi controllo che qi devoo essere solo due: comndo e argomento fine.non puoi avere 3 o 3 argomenti.
		{
			lenvnode = create_lenvnode(matrix[1]);
			if (lenvnode)
				printf("il nodo esiste. Il name è %s, mentre il valore è %s\n", lenvnode->name, lenvnode->value);
			listappend_envnode(lenvnode, &lenvlist);
			if (lenvlist)
				printf("finalmente la lista estiste\n");
			else
				printf("attenzione ho aggiunto un nodo ma c'è qualcosa che non va: la lista non esista\n");
		}
		else if (strcmp(matrix[0], "env") == 0)
		{//poi aggiungi controllo che qi devoo essere solo due: comndo e argomento fine.non puoi avere 3 o 3 argomenti.
			//printf("vado a stampare la lista\n");
			
			printenvlist(genvlist);
		}
		else if (strcmp(matrix[0], "lenv") == 0)
		{//poi aggiungi controllo che qi devoo essere solo due: comndo e argomento fine.non puoi avere 3 o 3 argomenti.
			//printf("vado a stampare la lista\n");
			printenvlist(lenvlist);
		}
		else if (strcmp(matrix[0], "export") == 0)
		{
			ft_export(matrix[1], lenvlist, &genvlist);


			
				//TODO export e unset
				/*
					in pratica devi passare la global list al child.
					ora. non devi gestire l'aggiunta delle variabili. quidi non ti interessa.
					devi lavorare sulle variabili già esistenti.
					queindi dovrai importare env e riprodurlo in un unlteriore.
					Ora hai env che è una matrice....fai tu! ma portala in lista.

					poi una volta avuta la tua lista interna. lavorerai su quella.
					avrai una seconda lista.
					identica.
					lavorerai principalemnte su quella
					Quando fai export quella variaile in local la aggungi allalista global..
					tu passi sempre la lista global.
					unset ti toglie la variabile da entrambe leliste.
				*/
			
		}
		else
		{
			cmd = create_cmd(matrix);
			//printf("AAAAAAA IO VENGO DAL MAIN\n");
			single_cmd_ex(cmd); //al momento senza redirections
		}
		/*else if (check_redirection_symbol(matrix) == THERE_IS_A_REDIRECTION)
		{
			printf("there is a redirection\n");
			cmd_before_single_outredirection(matrix);
		}
	*/

		/*********************************final part*******************************************/
		i = 0;
		while (matrix[i])
		{
			free(matrix[i]);
			matrix[i] = NULL;
			i++;
		}
		free(matrix);
		matrix = NULL;
		free(inputstr);
	}
	return (0);
}
