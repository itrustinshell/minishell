#include "../minishell.h"

/*operations with output and append redirections*/
void	oa_redirops(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist)
		return ;
	if (oa_rediropen(redirlist) == 0)
		return ;
	redirnode = oa_redirlast(redirlist);
	if (!redirnode)
		return ;
	oa_redirwrite(redirnode);
}

/*If exist an input redirection, then the indicated
file is checked with "access". If it exists, it is
open, and the input is redirected to his fd.
RETURN: returns 0 only if user indicates a file that 
doesn't exist. In all the other cases it returns 1,
even if there are no redirection.
Indeed, if there is no input redirection, no file is 
created and the stdin just is not redirect to any fd.
*/
int	i_redirops(t_redir *redirlist, int saved_stdout)
{
	int		fd;
	t_redir	*latest_input_redir;
	int		ret;

	ret = 1;
	if (!redirlist)
		return (ret);
	latest_input_redir = i_redirlast(redirlist);
	if (!latest_input_redir)
		return (2);
	if (access(latest_input_redir->file, F_OK) == 0)
	{
		fd = open(latest_input_redir->file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		dup2(saved_stdout, STDOUT_FILENO);
		printf("ERROREEEEEEEEE\n");
		ret = 0;
	}
	return (ret);
}

/*the whole input-output-append operations*/
int	ioa_redirops(t_redir *redirlist, int saved_stdout)
{
	if (i_redirops(redirlist, saved_stdout) == 0)
		return (0);
	oa_redirops(redirlist);
	return (1);
}
