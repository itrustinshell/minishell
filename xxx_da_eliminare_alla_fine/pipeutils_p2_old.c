/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeutils_p2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:37:04 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 19:37:08 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*exec builtins or external cmd*/
void	ft_execve(t_cmd *tmp_cmdlist, t_env *genvlist, char **envp, int *exit_code)
{
	char	**envlist;
	//if the execution of a builtin was succesfully done that's all.
	if (builtinex(tmp_cmdlist, &genvlist, exit_code) == 1)
		return ;
	//There was no built-in, so i go to find the path of the external cmd
	tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
	//i trasform the list of env in char **env, iot to pass them to execve param.
	envlist = litoma(genvlist);
	//i call execve. Previously in the code I made operations with redirection
	//it means that when i execute execve, it knows who is its input and 
	//where output (these are not info to pass via args_ execve knows how to do it
	// so it applies your changes in redirections stream.)


	//execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);		
	(void)envp;
	execve(tmp_cmdlist->path, tmp_cmdlist->args, envlist);

}

/*fork along the pipes*/
void	pipefork(int **pipematrix, t_cmd *cmdlist, 
		int i, int cmdlist_len, t_env **env, char **envp, int *exit_code)
{
	int	pid;
	int	saved_stdout;
	int	n_heredoc;
	int	ret;
	t_cmd *tmp_cmdlist;

	tmp_cmdlist = cmdlist;
	pid = fork();
	saved_stdout = dup(STDOUT_FILENO);
	if (pid == 0)
	{
		n_heredoc = 0;
		if (i > 0) 
		{
			piperead(pipematrix, i);
			
			ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
			if (ret == 0) 
				exit(1);
		}
		if (tmp_cmdlist->next)
		{
			pipewrite(pipematrix, i);
			ret = ihoa_redirops(tmp_cmdlist->redirlist, saved_stdout);
			printf("sono stati rilevati %d heredoc\n", n_heredoc);
			if (ret == 0)
				exit(1);			
		}
		pipeclose(pipematrix, cmdlist_len);
		ft_execve(tmp_cmdlist, *env, envp, exit_code);
		exit(1);
	}
}
