#include "../minishell.h"
#include <string.h>

int **generate_array_of_pipes_with_fd(int num_of_cmd)
{
	int i;
	int **pipe_array;

	pipe_array = (int **)malloc((num_of_cmd - 1) * sizeof(int *)); 
	i = 0;
	while (i < num_of_cmd - 1)
	{
		pipe_array[i] = (int *)malloc(2 * sizeof(int));
		i++;
	}
	i = 0;
	while (i < num_of_cmd)
	{
		pipe(pipe_array[i]);
		i++;
	}

	return (pipe_array);
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
			return (THERE_IS_A_PIPE);
	}
	return (THERE_IS_NOT_A_PIPE);
}

t_command *create_commandnode_for_pipe(char **tokenmatrix, int pipe_index, int generictoken_index)
{
	int i;
	int cmd_index;
	t_command *commandnode;

	commandnode = (t_command *)malloc(sizeof(t_command));
	cmd_index = pipe_index - generictoken_index;
	commandnode -> cmd = strdup(tokenmatrix[cmd_index]);
	
	/*arguments*/
	commandnode -> args = (char **)malloc((pipe_index - cmd_index + 1) * sizeof(char*));
	i = -1;
	while ((cmd_index + (++i)) < pipe_index)
		commandnode -> args[i] = strdup(tokenmatrix[cmd_index + i]);	
	commandnode->args[i] = NULL;
	commandnode->prev = NULL;
	commandnode->next = NULL;
	printf("created node\n");
	return (commandnode);
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
	if (*commandlist == NULL)
		*commandlist = commandnode;
	else
	{
		last_commandnode = find_last_commandnode(*commandlist);
		commandnode -> prev = last_commandnode;
		last_commandnode -> next = commandnode;
		commandnode -> next = NULL;
	}
}

void commandlist_generation(char **tokenmatrix, int *pipeindex, int *generictokenindex, t_command **commandlist)
{
	t_command *commandnode;
	int pipe_index;
	int generictoken_index;

	pipe_index = *pipeindex;
	generictoken_index = *generictokenindex;
	pipe_index = pipe_index + generictoken_index;
	commandnode = create_commandnode_for_pipe(tokenmatrix, pipe_index, generictoken_index);
	append_to_commandlist(commandnode, commandlist);
	*pipeindex = pipe_index;
	*generictokenindex = generictoken_index;
}

/*va a generare una lista di comandi quando ci sono le pipe*/
t_command *pipe_management(char **tokenmatrix)
{
	int pipe_index;
	int generictoken_index;
	t_command *commandlist;

	commandlist = NULL;
	pipe_index = -1;
	generictoken_index = 0;
	while (tokenmatrix[++pipe_index + (generictoken_index)] != NULL)
	{
		while (tokenmatrix[pipe_index + (generictoken_index)] && tokenmatrix[pipe_index + generictoken_index][0] != PIPE)
			generictoken_index++;
		if (tokenmatrix[pipe_index + generictoken_index] && tokenmatrix[pipe_index + generictoken_index][0] == PIPE)
		{
			commandlist_generation(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			generictoken_index = 0;
			continue;
		}
		else
		{
			commandlist_generation(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			break;
		}
	}
//	test_stampa_args(commandlist);
	return commandlist;
}

