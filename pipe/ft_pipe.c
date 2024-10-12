#include "../minishell.h"
#include <string.h>

int ft_strlen(char *str)
{
	int i;
	
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *strjoin(char *str, char *str_toadd)
{
	int lenstr;
	int len_strtoadd;
	char *str_toret;
	int i;
	int j;

	lenstr = ft_strlen(str);
	len_strtoadd = ft_strlen(str_toadd);
	str_toret = (char *)malloc((lenstr + len_strtoadd + 1) * sizeof(char));
	i = 0;
	while(str[i])
	{	
		str_toret[i] = str[i];
		i++;
	}
	j = 0;
	while(str_toadd[j])
	{
		str_toret[i + j] = str_toadd[j];
		j++;
	}
	return str_toret;
}

int check_if_there_is_at_least_one_pipe(char **tokenmatrix)
{
	int	i;

	i = 0;
	while (tokenmatrix[i])
	{
		if (tokenmatrix[i][0] != PIPE)
			i++;
		else
		{
			printf("Hei, there is a pipe here!\n"); //TODO: delete this line
			return (THERE_IS_A_PIPE);
		}
	}
	return (THERE_IS_NOT_A_PIPE);
}

t_command *create_commandnode_for_pipe(char **tokenmatrix, int pipe_index, int generictoken_index)
{
	printf("pipe_index:%d, generictoken_index:%d\n", pipe_index, generictoken_index);
	int i;
	int cmd_index;
	t_command *commandnode;

	commandnode = (t_command *)malloc(sizeof(t_command));
	cmd_index = pipe_index - generictoken_index;
	commandnode -> cmd = strdup(tokenmatrix[cmd_index]);
	commandnode -> args = (char **)malloc((pipe_index - cmd_index + 1) * sizeof(char*)); //il primo arg è il comando stesso
	i = -1;
	while ((cmd_index + (++i)) < pipe_index)
		commandnode -> args[i] = strdup(tokenmatrix[cmd_index + i]);	
	commandnode->args[i] = NULL;
	commandnode->prev = NULL;
	commandnode->next = NULL;
	printf("created node\n");
	return (commandnode);
}

/*al momento la funzione fa questo
riempie una lista statica di struct t_command
bisogna inserire anche l'ultima lettura
bisogna eeventulamente separare meglio i token
 */

void test_stampa_args(t_command *commandlist)
{
	int i;
	printf("andiamo a stampare il risultato\n");
	
	while (commandlist -> next != NULL)
	{
		printf("cmd: %s\n", commandlist -> cmd);
		i = 0;
		while (commandlist -> args[i])
		{
			printf("argument: %s\n", commandlist -> args[i]);
			i++;
		}
		commandlist = commandlist -> next;
	}
	printf("this last cmd: %s\n", commandlist-> cmd);
	i = 0;
	while (commandlist -> args[i])
	{
		printf("argument: %s\n", commandlist -> args[i]);
		i++;
	}
	printf ("it was already null\n");
}


t_command *find_last_commandnode(t_command *commandlist)
{
	t_command *current;
	
	current = commandlist;
	while (current -> next != NULL)
		current = current -> next;
	return (current);
}

void append_to_commandlist(t_command *commandnode, t_command **commandlist)
{
	t_command *last_commandnode;
	printf("siaomo nel cuore dell'append\n");
	if (*commandlist == NULL)
	{	
		printf("eh si adesso commandlist è nullo\n");
		*commandlist = commandnode;
	}
	else
	{
		last_commandnode = find_last_commandnode(*commandlist);
		commandnode -> prev = last_commandnode;
		last_commandnode -> next = commandnode;
		commandnode -> next = NULL;
	}
}

void pipe_management(char **tokenmatrix)
{
	int pipe_index;
	int generictoken_index;
	int c;
	t_command *commandnode;
	t_command *commandlist;

	commandlist = NULL;
	c = -1;
	pipe_index = -1;
	generictoken_index = -1;
	while (tokenmatrix[++pipe_index + (++generictoken_index)] != NULL)
	{
		while (tokenmatrix[pipe_index + generictoken_index] && tokenmatrix[pipe_index + generictoken_index][0] != PIPE)
			generictoken_index++;
		if (tokenmatrix[pipe_index + generictoken_index] && tokenmatrix[pipe_index + generictoken_index][0] == PIPE)
		{
			pipe_index = pipe_index + generictoken_index;
			commandnode = create_commandnode_for_pipe(tokenmatrix, pipe_index, generictoken_index);
			printf("andiamoa ad appendere\n");
			append_to_commandlist(commandnode, &commandlist);
			printf("abbiamo appeso\n");
			generictoken_index = 0;
			continue;
		}
		else
		{
			pipe_index = pipe_index + generictoken_index;
			commandnode = create_commandnode_for_pipe(tokenmatrix, pipe_index, generictoken_index);
			append_to_commandlist(commandnode, &commandlist);
			printf("sarebbe l'ultimo\n");
			break;
		}
	}
	printf("faccio una prova: %s\n", commandlist->next->cmd);
	
	int f = 0;
	t_command *tmp;
	tmp = commandlist;
	while (tmp->next)
	{
		printf("tmp non è null\n");
		tmp = tmp->next;
		f++;
	}
	printf("i = %d\n", f);
	test_stampa_args(commandlist);
}

