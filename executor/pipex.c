/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:46:22 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 23:16:41 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*execute pipes
stats all forks in a while, then the father waits.
*/

int	pipex(t_pipex_data *data, int *exit_code)
{
	int		i;
	t_cmd	*tmp_cmdlist;
	int		status;

	tmp_cmdlist = data->cmdlist;
	if (!tmp_cmdlist)
		return (0);
	i = -1;
	while (++i < data->cmdlist_len)
	{
		if (i > 0)
			tmp_cmdlist = tmp_cmdlist->next;
		pipefork(data->pipematrix, tmp_cmdlist, i, data->cmdlist_len,data->env, data->envp, exit_code);
	}
	pipeclose(data->pipematrix, data->cmdlist_len);
	i = 0;
	while (i < data->cmdlist_len)
	{
		wait(&status);
		*exit_code = WEXITSTATUS(status);
		i++;
	}
	return (0);
}
