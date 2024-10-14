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
