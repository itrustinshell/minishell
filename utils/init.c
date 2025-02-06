#include "../minishell.h"

/* Inizializza la struttura t_cmd
	char				*cmd;
	char				**args;
	char				*path;
	struct s_cmd		*next;
	t_redir				*redirlist;
	int					argc;
 */

void cmdinit(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->next = NULL;
	cmd->redirlist = NULL;
	cmd->argc = 0;
}




/* Inizializza la struttura t_redir */
void redirinit(t_redir *node)
{
	node->type = 0;
    node->file = NULL;
    node->delimiter = NULL;
    node->heredoclist = NULL;
    node->next = NULL;
}