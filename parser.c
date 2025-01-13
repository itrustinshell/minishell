#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "libft/libft.h"

#define TRUE 1
#define FALSE 0
#define BOOL char
#define PIPE '|'

#define OUT_REDIR 1
#define INPUT_REDIR 2
#define APPEND_REDIR 3
#define HEREDOC 4

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

typedef struct s_token
{
	unsigned int	len;
	char			*value;
	enum token_type	type;
}	t_token;

typedef struct s_heredoc
{
	char				*input;
	struct s_heredoc	*next;
} t_heredoc;

typedef struct s_redir
{
	int				type; //vedi le macro
	char			*file; //if ioa
	char			*delimiter; //if heredoc
	t_heredoc		*heredoclist;
} t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_list			*redirlist;
	int				argc;
}	t_cmd;

t_redir	*new_redir(char *filename, int type)
{
	t_redir	*redir;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = filename;
	return (redir);
}

BOOL	has_command(t_list *tokens)
{
	t_list	*node;

	if (!tokens)
		return (FALSE);
	node = tokens;
	while (node)
	{
		if (((t_token *)(node->content))->type == COMMAND)
			return (TRUE);
		node = node->next;
	}
	return (FALSE);
}

void	get_token_type(t_token *token, t_list *tokens)
{
	t_list	*last_node;
	t_token	*last_token;

	if (token->len == 2 && ft_strncmp(token->value, "<<", 2) == 0)		// DOUBLE LEFT REDIRECTION
		token->type = DOUBLE_LEFT;
	else if (token->len == 2 && ft_strncmp(token->value, ">>", 2) == 0)	// DOUBLE RIGHT REDIRECTION
		token->type = DOUBLE_RIGHT;
	else if (token->len == 1 && ft_strncmp(token->value, "<", 1) == 0)	// LEFT REDIRECTION
		token->type = LEFT;
	else if (token->len == 1 && ft_strncmp(token->value, ">", 1) == 0)	// RIGHT REDIRECTION
		token->type = RIGHT;
	// TODO: remember to add other bash symbols
	// TODO: add enum for heredoc and append arguments
	else {
		last_node = ft_lstlast(tokens);
		if (last_node)
			last_token = (t_token *)last_node->content;
		if (last_node && (last_token->type == DOUBLE_LEFT || last_token->type == LEFT))			// DOUBLE LEFT REDIRECTION ARGUMENT
			token->type = LEFT_ARG;
		else if (last_node && (last_token->type == DOUBLE_RIGHT || last_token->type == RIGHT))	// DOUBLE RIGHT REDIRECTION ARGUMENT
			token->type = RIGHT_ARG;
		else if(last_node && (last_token->type == COMMAND || last_token->type == ARG))			// ARGUMENTS
			token->type = ARG;
		else if (!last_node || !has_command(tokens))											// COMMAND
			token->type = COMMAND;
	}
	//printf("Token of type %d\n", token->type);
}

int	count_args(t_list *tokens)
{
	t_token	*current_tok;
	int	arguments;

	arguments = 0;
	while (tokens)
	{
		current_tok = (t_token *)tokens->content;
		if (current_tok->type == ARG)
			arguments++;
		tokens = tokens->next;
	}
	return (arguments);
}

t_cmd	*create_command(t_list *tokens)
{
	t_list		*current_node;
	t_token		*current_tok;
	t_cmd		*cmd;
	int			token_size;

	cmd = (t_cmd *)calloc(1, sizeof(t_cmd));
	current_node = tokens;
	cmd->args = (char **)calloc(count_args(tokens) + 1, sizeof(char *));
	cmd->args[count_args(tokens)] = NULL;

	while (current_node)
	{
		current_tok = (t_token *)current_node->content;
		if (current_tok->type == COMMAND)
		{
			token_size = ft_strlen(current_tok -> value);
			cmd->cmd = (char *)calloc(ft_strlen(current_tok -> value) + 1, sizeof(char));
			ft_strncpy(cmd->cmd, current_tok->value, token_size + 1);
			printf("Found Command: %s\n", cmd->cmd);
		}
		else if (current_tok->type == ARG)
		{
			cmd->args[cmd->argc] = calloc(current_tok->len + 1, sizeof(char));
			ft_strncpy(cmd->args[cmd->argc++], current_tok->value, current_tok->len); 
		}
		//TODO: Add append and heredoc
		else if (current_tok->type == LEFT_ARG)
		{
			token_size = ft_strlen(current_tok -> value);
			t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), INPUT_REDIR);
			ft_strncpy(redir->file, current_tok->value, token_size + 1);
			ft_lstadd_back(&cmd->redirlist, ft_lstnew(redir));
			printf("Found left redirection argument %s\n", 
				((t_redir *)(ft_lstlast(cmd->redirlist)->content))->file);
		}
		else if (current_tok->type == RIGHT_ARG)
		{
			token_size = ft_strlen(current_tok -> value);
			t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), OUT_REDIR);
			ft_strncpy(redir->file, current_tok->value, token_size + 1);
			ft_lstadd_back(&cmd->redirlist, ft_lstnew(redir));
			printf("Found right redirection argument %s\n", 
				((t_redir *)(ft_lstlast(cmd->redirlist)->content))->file);
		}
		current_node = current_node->next;
	}
	return (cmd);
}

BOOL	is_valid_arg_char(char c)
{
	return (!ft_isspace(c) && !ft_strchr("|<>\"\'$", c));
}

void	expand(char **env_var)
{
	char *expanded;

	if (!env_var || !*env_var)
		return ;
	expanded = getenv(*env_var);
	if (!expanded)
	{
		*env_var = (char *)calloc(1, sizeof(char));
		*env_var[0] = '\0';
	}
	else
	{
		*env_var = (char *)calloc(ft_strlen(expanded), sizeof(char));
		ft_strncpy(*env_var, expanded, ft_strlen(expanded));
	}
}

t_cmd	*parse_command(char *command_string)
{
	char			*beginning;
	t_token			*token;
	t_list			*tokens;

	tokens = NULL;
	while (*command_string)
	{
		beginning = NULL;
		token = (t_token *)calloc(1, sizeof(t_token));
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

void	parse_input(char *input)
{
	char		**command_strings;
	t_cmd		*cmd;

	command_strings = ft_split(input, PIPE);
	while (*command_strings)
	{
		printf("Parsing cmd %s\n", *command_strings);
		cmd = parse_command(*command_strings++);
		printf("Found command: %s\nRedirections:\n", cmd->cmd);
		ft_lstiter(cmd->redirlist, print_redir);
	}
}

int main()
{
	const char	*prompt;
	char		*input;

	prompt = "minishell > ";
	while (1)
	{
		input = readline(prompt);
		parse_input(input);

		free(input);
	}
	return (0);
}
