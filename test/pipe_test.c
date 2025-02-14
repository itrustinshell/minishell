/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:33:36 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 15:48:43 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Funzione per stampare gli argomenti di una lista di comandi */
void	test_stampa_args(t_cmd *commandlist)
{
	int	i;

	while (commandlist->next != NULL)
	{
		printf("this is the command: %s\n", commandlist->cmd);
		i = 0;
		while (commandlist->args[i])
		{
			printf("argument: %s\n", commandlist->args[i]);
			i++;
		}
		commandlist = commandlist->next;
	}
	printf("this last cmd: %s\n", commandlist->cmd);
	i = 0;
	while (commandlist->args[i])
	{
		printf("argument: %s\n", commandlist->args[i]);
		i++;
	}
	printf("it was already null\n");
}

/* Funzione per stampare la lista di comandi */
void	print_list(t_cmd *commandlist)
{
	t_cmd	*current;
	int		a;

	printf("i'm printing command list\n");
	current = commandlist;
	a = 0;
	while (current)
	{
		printf("cmd %d: %s\n", a, current->cmd);
		current = current->next;
		a++;
	}
}

/* Funzione di debug per stampare comandi, argomenti e redirazioni 
void	printlist(t_cmd *cmdlist)
{
	t_cmd		*tmp_cmdlist;
	t_redir		*tmp_redirlist;
	int			m;

	printf("\nDEBUG: printing list of cmd and redir\n");
	m = 0;
	tmp_cmdlist = cmdlist;
	while (tmp_cmdlist)
	{
		if (tmp_cmdlist->args)
		{
			printf("ecco gli argomenti di %s:\n", tmp_cmdlist->cmd);
			m = -1;
			while (tmp_cmdlist->args[++m])
				printf("args: %s\n", tmp_cmdlist->args[m]);
		}
		if (tmp_cmdlist->redirlist)
		{
			printf("ecco le redirections di %s:\n", tmp_cmdlist->cmd);
			tmp_redirlist = tmp_cmdlist->redirlist;
			while (tmp_redirlist)
			{
				if (tmp_redirlist->type == INPUT_REDIRECTION
					|| tmp_redirlist->type == OUTPUT_REDIRECTION
					|| tmp_redirlist->type == APPEND_REDIRECTION)
				{
					if (tmp_redirlist->type == INPUT_REDIRECTION)
						printf("inputredir: ");
					else if (tmp_redirlist->type == OUTPUT_REDIRECTION)
						printf("outputredir: ");
					else if (tmp_redirlist->type == APPEND_REDIRECTION)
						printf("appendredir: ");
					printf("%s\n", tmp_redirlist->file);
				}
				else if (tmp_redirlist->type == HEREDOC)
				{
					printf("heredoc: ");
					printf("%s\n", tmp_redirlist->delimiter);
				}
				tmp_redirlist = tmp_redirlist->next;
			}
		}
		else
			printf("non ci sono redir\n");
		tmp_cmdlist = tmp_cmdlist->next;
	}
}
*/