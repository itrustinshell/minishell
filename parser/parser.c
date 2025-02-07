/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:29:03 by dpalmese          #+#    #+#             */
/*   Updated: 2025/01/31 15:55:53 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "../minishell.h"

#define TRUE 1
#define FALSE 0

t_redir	*new_redir(char *filename, int type)
{
	t_redir	*redir;

	redir = (t_redir *)calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = filename;
	redir->next = NULL;
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
		if (((t_tkn *)(node->content))->type == COMMAND)
			return (TRUE);
		node = node->next;
	}
	return (FALSE);
}

void	get_token_type(t_tkn *token, t_list *tokens)
{
	t_list			*last_node;
	//t_tkn			*last_token;
	enum token_type	type;

	if (tkn_is_redir(token))
		set_redir_tkn(token);
	else
	{
		last_node = ft_lstlast(tokens);
		if (last_node)
			type = ((t_tkn *)last_node->content)->type;
		if (last_node && type == LEFT)
			token->type = LEFT_ARG;
		else if (last_node && (type == DOUBLE_LEFT))
			token->type = HEREDOC_ARG;
		else if (last_node && type == RIGHT)
			token->type = RIGHT_ARG;
		else if (last_node && type == DOUBLE_RIGHT)
			token->type = APPEND_ARG;
		else if (last_node && (type == COMMAND || type == ARG))
			token->type = ARG;
		else if (!last_node || !has_command(tokens))
			token->type = COMMAND;
	}
}

int	count_args(t_list *tokens)
{
	t_tkn	*current_tok;
	int		arguments;

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

t_cmd	*create_command(t_list *tokens)
{
	t_list	*current_node;
	t_tkn	*current_tok;
	t_cmd	*cmd;
	int		token_size;

	cmd = (t_cmd *)calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmdinit(cmd); //ho aggiunto una inizializzazione; Ha risolto i problemi di memrory leak in fase di testing
	current_node = tokens;
	cmd->args = (char **)calloc(count_args(tokens) + 2, sizeof(char *));
	if (!cmd->args)
	{
		free_cmd(cmd);
		return (NULL);
	}
	cmd->args[count_args(tokens) + 1] = NULL;
	while (current_node)
	{
		current_tok = (t_tkn *)current_node->content;
		if (current_tok->type == COMMAND)
		{
			token_size = ft_strlen(current_tok->value);
			cmd->cmd = (char *)calloc(token_size + 1, sizeof(char));
			if (!cmd->cmd)
			{
				free_cmd(cmd);
				return (NULL);
			}
			ft_strncpy(cmd->cmd, current_tok->value, token_size);
			cmd->path = get_cmdpath(cmd->cmd);	
			cmd->args[0] = ft_strdup(cmd->cmd);
		}
		else if (current_tok->type == ARG)
		{
			cmd->args[cmd->argc + 1] = calloc(current_tok->len + 1, sizeof(char));
			if (!cmd->args[cmd->argc + 1])
			{
				free_cmd(cmd);
				return (NULL);
			}
			ft_strncpy(cmd->args[cmd->argc + 1], current_tok->value, current_tok->len);
			cmd->argc++;
		}
		else if (current_tok->type == LEFT_ARG)
		{
			token_size = ft_strlen(current_tok->value);
			t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), INPUT_REDIRECTION);
			if (!redir || !redir->file)
			{
				free_cmd(cmd);
				return (NULL);
			}
			ft_strncpy(redir->file, current_tok->value, token_size);
			listappend_redir(redir, &cmd->redirlist);
		}
		else if (current_tok->type == RIGHT_ARG)
		{
			token_size = ft_strlen(current_tok->value);
			t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), OUTPUT_REDIRECTION);
			if (!redir || !redir->file)
			{
				free_cmd(cmd);
				return (NULL);
			}
			ft_strncpy(redir->file, current_tok->value, token_size);
			listappend_redir(redir, &cmd->redirlist);
		}
		else if (current_tok->type == HEREDOC_ARG)
		{
			token_size = ft_strlen(current_tok->value);
			t_redir *redir = new_redir(NULL, HEREDOC);
			redir->delimiter = (char *)calloc(token_size + 1, sizeof(char));
			if (!redir || !redir->delimiter)
			{
				return (NULL);
				free_cmd(cmd);
			}
			ft_strncpy(redir->delimiter, current_tok->value, token_size);
			listappend_redir(redir, &cmd->redirlist);
		}
		else if (current_tok->type == APPEND_ARG)
		{
			token_size = ft_strlen(current_tok->value);
			t_redir *redir = new_redir((char *)calloc(token_size + 1, sizeof(char)), APPEND_REDIRECTION);
			if (!redir || !redir->file)
				return (NULL);
			ft_strncpy(redir->file, current_tok->value, token_size);
			listappend_redir(redir, &cmd->redirlist);
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
	char	*expanded;

	if (!env_var || !*env_var)
		return ;
	expanded = getenv((char *)*env_var);
	if (!expanded)
	{
		free(*env_var);
		*env_var = (char *)calloc(1, sizeof(char));
		*env_var[0] = '\0';
	}
	else
	{
		free(*env_var);
		*env_var = (char *)ft_calloc(ft_strlen(expanded) + 1, sizeof(char));
		ft_strncpy(*env_var, expanded, ft_strlen(expanded));
	}
}


/* Funzione per liberare un token */
void free_tkn(void *token)
{
	t_tkn *t = (t_tkn *)token;
	if (t)
	{
		free(t->value);
		free(t);
	}
}

void free_tokenslist(t_list *tokens)
{
	t_list *tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->content)
		{
			if (((t_tkn *)tokens->content)->value)
				free(((t_tkn *)tokens->content)->value);
			free(tokens->content);
		}
		free(tokens);
		tokens = tmp;
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
			command_string++;
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
			//printf("trovato\n");
			//printf("ecco cosa viene dopo: %c\n", *(command_string + 1));
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
			token->len = ft_strlen(token->value);
		}
		else if (*command_string)
		{
			//printf("ecco command_string: %c\n", *(command_string));
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
	
	//return(create_command(tokens)); //tokens qui non veniva liberato. dopo aver comentato e aggiungto 
	//le tre righe sotto, ho risolto quel memory leak.

	t_cmd *cmdlist = create_command(tokens);
	free_tokenslist(tokens);
	return (cmdlist);

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
    command_strings = ft_split(input, PIPE); //TODO: use the right split
    if (!command_strings)
        return (NULL);
    
    i = 0;
    while (command_strings[i])
    {
        current_cmd = parse_command(command_strings[i]);
        if (!current_cmd)
        {
            free_matrix(command_strings);
            free_cmd(cmd_list);
            return (NULL);
        }
        listappend_command(current_cmd, &cmd_list);
        i++;
    }
    free_matrix(command_strings);
    return (cmd_list);
}
