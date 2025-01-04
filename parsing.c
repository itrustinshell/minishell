#include "minishell.h"

t_command	*parsing(char **tokenmatrix)
{
	int			i;
	int			j;
	int			t;
	t_command	*cmdnode;
	t_command	*cmdlist;
	t_redir		*redirnode;

	redirnode = NULL;
	cmdlist = NULL;
	cmdnode = NULL;
	i = 0;
	while (tokenmatrix[i] != NULL)
	{	
		cmdnode = (t_command *)malloc(sizeof(t_command));
		init_cmd(cmdnode);
		cmdnode->redirlist = NULL;
		//cmdnode->envp = envp;
		listappend_command(cmdnode, &cmdlist);
		last_cmdnode(cmdlist)->cmd = tokenmatrix[i]; //TODO verifica se cé bisogno di strdup
		// if (strcmp(cmdnode->cmd, "echo") == 0)
		// {
		// 	cmdnode->builtin = 1;
		// }
		j = 0;
		while (tokenmatrix[i + j] && strcmp(tokenmatrix[i + j], ">") != 0 && strcmp(tokenmatrix[i + j], "<") != 0 && strcmp(tokenmatrix[i + j], ">>") != 0 && tokenmatrix[i + j][0] != PIPE)
			j++;
		if (j)
		{
			last_cmdnode(cmdlist)->args = (char **)malloc((j + 1) * sizeof(char *));
			t = 0;
			while(t < j)
			{
				last_cmdnode(cmdlist)->args[t] = tokenmatrix[i + t]; //verifica se non e'unn strdup
				t++;
			}
			last_cmdnode(cmdlist)->args[t] = NULL;
		}
		i = i + j;
		while (tokenmatrix[i] && tokenmatrix[i][0] != PIPE)
		{
			if (strcmp(tokenmatrix[i], ">") == 0 || strcmp(tokenmatrix[i], "<") == 0 || strcmp(tokenmatrix[i], ">>") == 0)
			{
				redirnode = (t_redir *)malloc(sizeof(t_redir));
				if ((strcmp(tokenmatrix[i], ">") == 0))
					redirnode->type = OUTPUT_REDIRECTION;
				if ((strcmp(tokenmatrix[i], "<") == 0))
					redirnode->type = INPUT_REDIRECTION;
				if ((strcmp(tokenmatrix[i], ">>") == 0))
					redirnode->type = APPEND_REDIRECTION;
				i++;
				redirnode->outredir_file = tokenmatrix[i];
				listappend_redir(redirnode, &last_cmdnode(cmdlist)->redirlist);
			}
			i++;
		}
		if (!tokenmatrix[i])
			break;
		i++;
	}
	return (cmdlist);
}
