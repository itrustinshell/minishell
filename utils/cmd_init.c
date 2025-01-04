#include "../minishell.h"

void init_cmd(t_command *cmd)
{
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->path = NULL;
    cmd->next = NULL;
    cmd->redirlist = NULL;
}