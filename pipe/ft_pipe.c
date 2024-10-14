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

	/*create command node*/
	commandnode = (t_command *)malloc(sizeof(t_command));
	/*assign cmd*/
	cmd_index = pipe_index - generictoken_index;
	commandnode -> cmd = strdup(tokenmatrix[cmd_index]);
	/*assaign argument arguments: pipe_index - cmd_index + 1 becouse arguments to pass to 
	 execve function have as first element the name of cmd (I put also a NULL to better
	 iterate later)..so if before pipe there are 4
	 elements, the first is the command the other 3 are arguments.
	 
	index:   0    1    2    3    4    5   6     7 
	tokens: cmd1 arg1 arg2 arg3  |   cmd2 arg1 arg2
	whll pipe_index = 4
	cmd_index = 0
	how much arguments to pass to execve: 4 - 0 + 1 = 5
	
	So if commandnode-args have a null terminator and include as first element the cmq itself
	 */
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


/*becouse of norm i got this function
the function is void so it modify something inside that reflects in the code (that is the list).
Thi function create a new command node.
Then the function append this newcreated commandnode to the list.
It updates the pipe_index passed byreference, to allow the next iteration to start from there.
 */
void commandnode_management(char **tokenmatrix, int *pipe_index, int *generictoken_index, t_command **commandlist)
{
	t_command *commandnode;

	*pipe_index = *pipe_index + *generictoken_index;
	commandnode = create_commandnode_for_pipe(tokenmatrix, *pipe_index, *generictoken_index);
	append_to_commandlist(commandnode, commandlist);
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
			commandnode_management(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			generictoken_index = 0;
			continue;
		}
		else
		{
			commandnode_management(tokenmatrix, &pipe_index, &generictoken_index, &commandlist);
			break;
		}
	}
//	test_stampa_args(commandlist);
	return commandlist;
}

