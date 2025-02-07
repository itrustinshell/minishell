#include "../minishell.h"

// Se non riusciamo ad allocare la stringa per il comando Amen
void    add_cmd(t_cmd *cmd, t_tkn *token)
{
	cmd->cmd = (char *)calloc(token->len + 1, sizeof(char));
	ft_strncpy(cmd->cmd, token->value, token->len);
	cmd->path = get_cmdpath(cmd->cmd);
	cmd->args[0] = ft_strdup(cmd->cmd);
}