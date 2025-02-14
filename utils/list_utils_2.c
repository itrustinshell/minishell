/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:41:13 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/14 15:47:16 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	listlen(t_cmd *list)
{
	int		i;
	t_cmd	*current;

	if (list == NULL)
		return (0);
	current = list;
	i = 0;
	while (current->next)
	{
		current = current->next;
		i++;
	}
	i++;
	return (i);
}
