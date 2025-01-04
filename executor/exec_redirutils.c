#include "../minishell.h"

/*find last occurrence indifferently among output or append redirections*/
t_redir *oa_redirlast(t_redir *redirlist)
{
	t_redir *ret;
	t_redir *tmp;

	if (!redirlist)
		return (NULL);
	tmp = redirlist;
	ret = NULL;
	while(tmp)
	{
		if (tmp->type == OUTPUT_REDIRECTION || tmp->type == APPEND_REDIRECTION)
			ret = tmp;
		tmp = tmp->next;
	}
	return (ret);
}

/*write in all output and append redirections*/
void oa_redirwrite(t_redir *redirnode)
{
	int		fd;
	t_redir	*tmp_redirnode;

	if (!redirnode)
		return;
	tmp_redirnode = redirnode;
	if (tmp_redirnode->type == OUTPUT_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (tmp_redirnode->type == APPEND_REDIRECTION)
	{
		fd = open(tmp_redirnode->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

/*open all output and append redirections*/
int oa_rediropen(t_redir *redirlist)
{
	int fd;
	int	output_append_exists;
	t_redir	*tmp_redirlist;

	output_append_exists = 0;
	tmp_redirlist = redirlist;
	while(tmp_redirlist)
	{
		if (tmp_redirlist->type == OUTPUT_REDIRECTION) 
		{
			output_append_exists = 1;
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			close(fd);
		}
		if (tmp_redirlist->type == APPEND_REDIRECTION)
		{
			output_append_exists = 1;
			fd = open(tmp_redirlist->outredir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
			close(fd);
		}
		tmp_redirlist = tmp_redirlist->next;
	}
	return (output_append_exists);
}

/*operatiosn with output and append redirections*/
void oa_redirops(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist)
		return;
	if (oa_rediropen(redirlist) == 0)
		return;
	redirnode = oa_redirlast(redirlist);
	if (!redirnode)
		return;
	oa_redirwrite(redirnode);
}

/*finds last input redirections*/
t_redir *i_redirlast(t_redir *redirlist)
{
	t_redir	*tmp_redirlist;
	t_redir	*ret;

	if (!redirlist)
		return (NULL);
	ret = NULL;
	tmp_redirlist = redirlist;
	while(tmp_redirlist)
	{
		if (tmp_redirlist->type == INPUT_REDIRECTION)
			ret = tmp_redirlist;
		tmp_redirlist = tmp_redirlist->next;
	}
	return (ret);
}

/*operations with input redirection*/
int	i_redirops(t_redir *redirlist, int saved_stdout)
{
	int	fd;
	t_redir	*latest_input_redir;
	int ret;

	ret = 1;
	if (!redirlist)
		return (ret);
	latest_input_redir = i_redirlast(redirlist);
	if (!latest_input_redir)
		return (ret);
	if (access(latest_input_redir->outredir_file, F_OK) == 0)
	{
		fd = open(latest_input_redir->outredir_file, O_RDONLY);
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
int ioa_redirops(t_redir *redirlist,  int saved_stdout)
{
	if (i_redirops(redirlist, saved_stdout) == 0)
		return(0);
	oa_redirops(redirlist);
	return (1);
}
