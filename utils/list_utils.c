#include "../minishell.h"

int number_of_nodes_in_a_list(t_command *list)
{
	int i;
	t_command *current;
	
	if (list == NULL)
		return (0);
	current = list;
	i = 0;
	while (current -> next)
	{
		current = current->next;
		i++;
	}
	i++;
	return (i);
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




