
#include "../minishell.h"
/*
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

void	handle_redirection(char **command_string, t_tkn *token)
{
	if (**command_string == '<')
	{
		if (*(*command_string + 1) == '<')
			token->len = 2;
		else
			token->len = 1;
		token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
		ft_strncpy(token->value, *command_string, token->len);
		*command_string += token->len;
	}
	else if (**command_string == '>')
	{
		if (*(*command_string + 1) == '>')
			token->len = 2;
		else
			token->len = 1;
		token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
		ft_strncpy(token->value, *command_string, token->len);
		*command_string += token->len;
	}
}

void	handle_quotes(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = ++(*command_string);
	while (**command_string && **command_string != '"')
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
	(*command_string)++;
}

void	handle_env_var(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = ++(*command_string);
	while (**command_string && is_valid_arg_char(**command_string))
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
	expand(&token->value);
}

void	handle_plain_text(char **command_string, t_tkn *token)
{
	char	*beginning;

	beginning = *command_string;
	while (is_valid_arg_char(**command_string) && **command_string)
	{
		(*command_string)++;
		token->len++;
	}
	token->value = (char *)ft_calloc(token->len + 1, sizeof(char));
	ft_strncpy(token->value, beginning, token->len);
}
*/