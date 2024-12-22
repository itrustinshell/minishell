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

t_command *last_cmdnode(t_command *commandlist)
{
	t_command *current;

	if (!commandlist)
		return NULL;
	current = commandlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

t_redir *find_last_commandnode_redir(t_redir *redirlist)
{
	t_redir *current;

	if (!redirlist)
		return NULL;
	current = redirlist;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void listappend_command(t_command *node, t_command **list)
{
	t_command *last_node;

	if (!node)
		return;

	if (*list == NULL)
	{
		//printf("Yes it is null now\n");
		*list = node;
		(*list)->next = NULL;
		//printf("hahaha: %s\n", (*list)->cmd);
	}
	else
	{
		//printf("it is no more null\n");
		last_node = last_cmdnode(*list);
		//printf("i found last node: %s\n", last_node->cmd);
		node->prev = last_node;
		last_node->next = node;
		node->next = NULL;
		//printf("hehehe: %s\n", (*list)->cmd);
	}
}

void listappend_redir(t_redir *node, t_redir **list)
{
	t_redir *last_node;

	//printf("it seems all ok \n");
	if (*list == NULL)
	{
		*list = node;
		//printf("listnode added succesfully!\n");
		(*list)->next = NULL;
		(*list)->prev = NULL;
	}
	else
	{
		last_node = find_last_commandnode_redir(*list);
	//	printf("the last added node was: %s\n", last_node->outredir_file);
		node->prev = last_node;
		last_node->next = node;
	//	printf("listnode added succesfully!\n");
	//	printf("I have just added: %s\n", node->outredir_file);
		node->next = NULL;
	}
}




