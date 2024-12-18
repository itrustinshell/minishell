#include "../minishell.h"

int listlen(t_command *list)
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

int listlenredir(t_redir *list)
{
	int i;
	t_redir *current;
	
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

t_redir *find_last_commandnode_redir(t_redir *commandlist)
{
	t_redir *current;
	
	current = commandlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void listappend_command(t_command *node, t_command **list)
{
	t_command *last_node;

	if (*list == NULL)
	{
		printf("Yes it is null now\n");
		*list = node;
		(*list)->next = NULL;
		printf("hahaha: %s\n", (*list)->cmd);
	}
	else
	{
		printf("it is no more null\n");
		last_node = find_last_commandnode(*list);
		printf("i found last node: %s\n", last_node->cmd);
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
		printf("hehehe: %s\n", (*list)->cmd);
	}
}

void listappend_redir(t_redir *node, t_redir **list)
{
	t_redir *last_node;

	printf("it seems all ok \n");
	if (*list == NULL)
		*list = node;
	else
	{
		last_node = find_last_commandnode_redir(*list);
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
	}
}




