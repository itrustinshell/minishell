/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_SACRO_del_BOSS.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:29:03 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/09 14:26:11 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "../minishell.h"

#define TRUE 1
#define FALSE 0

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
	t_tkn	*curr_tok;
	t_cmd	*cmd;

	cmd = new_cmd(count_args(tokens) + 2);
	if (!cmd)
		return (NULL);
	current_node = tokens;
	cmd->args[count_args(tokens) + 1] = NULL;
	while (current_node)
	{
		curr_tok = (t_tkn *)current_node->content;
		if (curr_tok->type == COMMAND)
			add_cmd(cmd, curr_tok);
		else if (curr_tok->type == ARG)
			add_arg(cmd, curr_tok);
		else if (curr_tok->type == LEFT_ARG)
			add_redir(cmd, curr_tok, INPUT_REDIRECTION);
		else if (curr_tok->type == RIGHT_ARG)
			add_redir(cmd, curr_tok, OUTPUT_REDIRECTION);
		else if (curr_tok->type == APPEND_ARG)
			add_redir(cmd, curr_tok, APPEND_REDIRECTION);
		else if (curr_tok->type == HEREDOC_ARG)
			add_heredoc(cmd, curr_tok);
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

t_cmd	*parse_command(char *command_string)
{
	char			*beginning;
	t_tkn			*token;
	t_list			*tokens;
	t_cmd			*cmd;

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
		else if(*command_string == '$' && *(command_string + 1) == '?' && *(command_string + 2) == '\0')
		{ //AAA questo blocco di esle e'stato aggiungo da largenzi per gestire echo $?
			//printf("stai bell\n");
			beginning = command_string;
			while (*command_string)
			{
				command_string++;
				token -> len++;
			}
			// Here i have a new token
			token->value = (char *)calloc(token->len + 1, sizeof(char));
			strncpy(token->value, beginning, token->len);
			//fine del blocco aggiunto da largenzi
		}
		else if (*command_string == '$' && *(command_string + 1) != '?')
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
		}
		else if (*command_string)
		{ //aaa anche in questo else if ho aggionto del codice perche'vanno gestite le espressioni del tipo $? +$?
			//printf("ecco command_string: %c\n", *(command_string));
			beginning = command_string;
			if (*(command_string + 1) == '?')
			{
				while (*command_string && *command_string != ' ')
				{
					command_string++;
					token -> len++;
				}
			}
			else //ho aggiunto questo else prrch' ho messo un elseif prtima... e dentro ho messo il blocco originale
			{
				while ((is_valid_arg_char(*command_string)) && *command_string)
				{
					command_string++;
					token -> len++;
				}
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
	/*TOKENS: deve essere fiato qui. Non si può terminare con return (create_command(tokens));
		a meno che non ci sia un modo per lierare contestualmente
	*/
	cmd = create_command(tokens);
	free_tokenslist(tokens);
	return (cmd);
}

void print_redir(void *redir_node)
{
	printf("%s\n", ((t_redir *)redir_node)->file);
}

	void free_command_structure(Command *cmd) {
    if (cmd) {
        free(cmd->args);  // Free dynamically allocated argument list
        free(cmd);        // Free the command struct itself
    }
}

// Call this function at the end of parse_input or where commands are no longer needed.


t_cmd *parse_input(char *input)
{
	char	**command_strings;
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;

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
		//printf("Oilloc o comand :%s %s\n", current_cmd->cmd, current_cmd->args[1]);
		i++;
	}
	free_matrix(command_strings);
	return (cmd_list);
}

