#include "../minishell.h"

/*scorre le outpu-redirections e append-redirections.
restituisce l'ultima che trova tra le due.
Se l'ultima è un outputredirection allora ritora outputredirections e basta.
Se l'utima è appendredirectione allora ritorna appendredirection e basta.*/
t_redir *find_latest_redirection_among_outpus_and_appends(t_redir *redirlist)
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

void ft_write_in_latest_outpu_or_append_redir(t_redir *redirnode)
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
			
int ft_open_all_output_and_append_redirections(t_redir *redirlist)
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

void output_append_management(t_redir *redirlist)
{
	t_redir	*redirnode;

	redirnode = NULL;
	if (!redirlist)
		return;
	if (ft_open_all_output_and_append_redirections(redirlist) == 0)
		return;
	redirnode = find_latest_redirection_among_outpus_and_appends(redirlist);
	if (!redirnode)
		return;
	ft_write_in_latest_outpu_or_append_redir(redirnode);
}

t_redir *find_latest_input_redirection(t_redir *redirlist)
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

int	get_input_from_latest_input_redir(t_redir *redirlist, int saved_stdout)
{
	int	fd;
	t_redir	*latest_input_redir;
	int ret;

	ret = 1;
	if (!redirlist)
		return (ret);
	latest_input_redir = find_latest_input_redirection(redirlist);
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

int input_output_routine(t_redir *redirlist,  int saved_stdout)
{
	if (get_input_from_latest_input_redir(redirlist, saved_stdout) == 0)
		return(0);
	output_append_management(redirlist);
	return (1);
}

int check_redirection_symbol(char **tokenmatrix) //cerca se c'è almeno una redirection
{
	int	i;

	i = 0;
	while (tokenmatrix[i])
	{
		if (tokenmatrix[i][0] != OUTPUT_REDIRECTION)
			i++;
		else
		{
			//printf("there is a pipe\n");
			return (THERE_IS_A_REDIRECTION);
		}
	}
	return (THERE_IS_NOT_A_REDIRECTION);
}