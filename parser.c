#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "minishell.h"

#define TRUE 1
#define FALSE 0
#define BOOL char

enum token_type
{
    INVALID,
    COMMAND,
    ARG,
    LEFT,
    RIGHT,
    DOUBLE_LEFT,
    DOUBLE_RIGHT,
    LEFT_ARG,
    RIGHT_ARG,
};

typedef struct s_tkn
{
    unsigned int    len;
    char           *value;
    enum token_type type;
} t_tkn;

t_redir *new_redir(char *filename, int type)
{
    t_redir *redir;

    redir = (t_redir *)calloc(1, sizeof(t_redir));
    if (!redir)
        return (NULL);
    redirinit(redir);
    redir->type = type;
    redir->file = filename;
    redir->next = NULL;
    return (redir);
}

BOOL has_command(t_list *tokens)
{
    t_list *node;

    if (!tokens)
        return (FALSE);
    node = tokens;
    while (node)
    {
        if (((t_tkn *)(node->content))->type == COMMAND)
            return (TRUE);
        node = node->next;
    }
    return (FALSE);
}

void get_token_type(t_tkn *token, t_list *tokens)
{
    t_list  *last_node;
    t_tkn *last_token;

    if (token->len == 2 && ft_strncmp(token->value, "<<", 2) == 0)
        token->type = DOUBLE_LEFT;
    else if (token->len == 2 && ft_strncmp(token->value, ">>", 2) == 0)
        token->type = DOUBLE_RIGHT;
    else if (token->len == 1 && ft_strncmp(token->value, "<", 1) == 0)
        token->type = LEFT;
    else if (token->len == 1 && ft_strncmp(token->value, ">", 1) == 0)
        token->type = RIGHT;
    else {
        last_node = ft_lstlast(tokens);
        if (last_node)
            last_token = (t_tkn *)last_node->content;
        if (last_node && (last_token->type == DOUBLE_LEFT || last_token->type == LEFT))
            token->type = LEFT_ARG;
        else if (last_node && (last_token->type == DOUBLE_RIGHT || last_token->type == RIGHT))
            token->type = RIGHT_ARG;
        else if(last_node && (last_token->type == COMMAND || last_token->type == ARG))
            token->type = ARG;
        else if (!last_node || !has_command(tokens))
            token->type = COMMAND;
    }
}

int count_args(t_list *tokens)
{
    t_tkn *current_tok;
    int      arguments;

    arguments = 0;
    while (tokens)
    {
        current_tok = (t_tkn *)tokens->content;
        if (current_tok->type == ARG)
            arguments++;
        tokens = tokens->next;
    }
    return (arguments);
}

t_cmd *create_command(t_list *tokens)
{
    t_list  *current_node;
    t_tkn *current_tok;
    t_cmd   *cmd;
    int      token_size;

    cmd = (t_cmd *)calloc(1, sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmdinit(cmd);
    current_node = tokens;
    cmd->args = (char **)calloc(count_args(tokens) + 2, sizeof(char *));
    if (!cmd->args)
        return (NULL);
    cmd->args[count_args(tokens) + 1] = NULL;

    while (current_node)
    {
        current_tok = (t_tkn *)current_node->content;
        if (current_tok->type == COMMAND)
        {
            token_size = ft_strlen(current_tok->value);
            cmd->cmd = (char *)calloc(token_size + 1, sizeof(char));
            if (!cmd->cmd)
                return (NULL);
            ft_strncpy(cmd->cmd, current_tok->value, token_size);
            cmd->path = get_cmdpath(cmd->cmd);
            cmd->args[0] = ft_strdup(cmd->cmd);
        }
        else if (current_tok->type == ARG)
        {
            cmd->args[cmd->argc + 1] = calloc(current_tok->len + 1, sizeof(char));
            if (!cmd->args[cmd->argc + 1])
                return (NULL);
            ft_strncpy(cmd->args[cmd->argc + 1], current_tok->value, current_tok->len);
            cmd->argc++;
        }
        else if (current_tok->type == LEFT_ARG)
        {
            token_size = ft_strlen(current_tok->value);
            t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), INPUT_REDIR);
            if (!redir || !redir->file)
                return (NULL);
            ft_strncpy(redir->file, current_tok->value, token_size);
            listappend_redir(redir, &cmd->redirlist);
        }
        else if (current_tok->type == RIGHT_ARG)
        {
            token_size = ft_strlen(current_tok->value);
            t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), OUT_REDIR);
            if (!redir || !redir->file)
                return (NULL);
            ft_strncpy(redir->file, current_tok->value, token_size);
            listappend_redir(redir, &cmd->redirlist);
        }
        current_node = current_node->next;
    }
    return (cmd);
}

BOOL is_valid_arg_char(char c)
{
    return (!ft_isspace(c) && !ft_strchr("|<>\"\'$", c));
}

void expand(char **env_var)
{
    char *expanded;

    if (!env_var || !*env_var)
        return;
    expanded = getenv(*env_var);
    if (!expanded)
    {
        free(*env_var);
        *env_var = (char *)calloc(1, sizeof(char));
        (*env_var)[0] = '\0';
    }
    else
    {
        free(*env_var);
        *env_var = (char *)calloc(ft_strlen(expanded) + 1, sizeof(char));
        ft_strncpy(*env_var, expanded, ft_strlen(expanded));
    }
}

t_cmd	*parse_command(char *command_string)
{
	char			*beginning;
	t_tkn			*token;
	t_list			*tokens;

	tokens = NULL;
	while (*command_string)
	{
		beginning = NULL;
		token = (t_tkn *)calloc(1, sizeof(t_tkn));
		while (ft_isspace(*command_string))
		{
			command_string++;
		}
		if (*command_string == 0)
			break ;

		if (*command_string == '<')
		{
			if (*(command_string + 1) == '<')
			{
				token -> len = 2;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  "<<", token->len);
				command_string += 2;
			}
			else
			{
				token -> len = 1;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  "<", token->len);
				command_string += 1;
			}
		}
		else if (*command_string == '>')
		{
			if (*(command_string + 1) == '>')
			{
				token -> len = 2;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  ">>", token->len);
				command_string += 2;
			}
			else
			{
				token -> len = 1;
				token->value = (char *)calloc(token->len + 1, sizeof(char));
				strncpy(token->value,  ">", token->len);
				command_string += 1;
			}
		}
		else if (*command_string == '"')
		{
			beginning = ++command_string;
			while (*command_string && *command_string != '"')
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			command_string++;
		}
		else if (*command_string == '$')
		{
			//printf("Found env variable\n");
			beginning = ++command_string;
			while (*command_string && is_valid_arg_char(*command_string))
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			expand(&token->value);
		}
		else if (*command_string)
		{
			beginning = command_string;
			while ((is_valid_arg_char(*command_string)) && *command_string)
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
		}
		//printf("Token: %s\n", token->value);
		get_token_type(token, tokens);
		ft_lstadd_back(&tokens, ft_lstnew(token));
	}
	//printf("Now we have the tokens, lets create a cmd to execute\n");
	return (create_command(tokens));
}

void print_redir(void *redir_node)
{
	printf("%s\n", ((t_redir *)redir_node)->file);
}

t_cmd *parse_input(char *input)
{
    char **command_strings;
    t_cmd *cmd_list;
    t_cmd *current_cmd;
    int   i;

    cmd_list = NULL;
    command_strings = ft_split(input, PIPE);
    if (!command_strings)
        return (NULL);
    
    i = 0;
    while (command_strings[i])
    {
        current_cmd = parse_command(command_strings[i]);
        if (!current_cmd)
        {
            ft_freematrix(command_strings);
            free_cmd(cmd_list);
            return (NULL);
        }
        listappend_command(current_cmd, &cmd_list);
        i++;
    }
    ft_freematrix(command_strings);
    return (cmd_list);
}