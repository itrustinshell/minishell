


void	print_heredoc_for_redir(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == HEREDOC && redir->heredoclist)
			print_heredoc_entries(redir->heredoclist);
		redir = redir->next;
	}
}

void	printallheredoclists(t_cmd *cmd)
{
	t_cmd	*tmp_cmdlist;

	tmp_cmdlist = cmd;
	while (tmp_cmdlist)
	{
		if (tmp_cmdlist->redirlist)
			print_heredoc_for_redir(tmp_cmdlist->redirlist);
		tmp_cmdlist = tmp_cmdlist->next;
	}
}