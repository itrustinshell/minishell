#include "minishell.h"
//fill token
/*
AAA quando dopo < metto solo una lettera ad es <d
non riconosce la d come token separato da <

 */
char **matrixalloc(int n_tokens, char *str)
{
	char **matrix_to_ret;

	matrix_to_ret = NULL;
	matrix_to_ret = (char **)malloc((n_tokens + 1) * sizeof(char *));
	if (!matrix_to_ret)
		return (NULL);
	return (matrix_to_ret);
}

char *create_token(char *str, int i, int len)
{
	char *token;
	int t;

	token = (char *)malloc((len + 1) * sizeof(char));
	//TODO: if ...null....
	t = 0;
	while (t < len)
	{
		token[t] = str[i + t];
		t++;
	}
	token[t] = '\0';
	return (token);
}

/*given a string, it calculates how many words it is composed of*/
char **create_tokenmatrix(char *str_to_tokenize, int n_tokens)
{
	char **tokens_matrix;
	int tmp;
	int i;
	int t;
	int len;
	tokens_matrix = matrixalloc(n_tokens, str_to_tokenize);
	if (!tokens_matrix)
		return (NULL);

	t = 0;
	i = 0;
	while (t < n_tokens)
	{
	
		while (str_to_tokenize[i] && ft_isspace(str_to_tokenize[i]))
			i++;
		len = 0;
		while (str_to_tokenize[i + len] && !ft_isspace(str_to_tokenize[i + len]) && !ft_issymbol(str_to_tokenize[i + len]))
			len++;
		if (len > 0)
		{
			tokens_matrix[t] = create_token(str_to_tokenize, i, len);
			i = i + len;
			t++;
		}
		tmp = i;
		/*    
			c i a o   < s i a o     \0
			0 1 2 3 4 5 6 7 8 9   10
					 i
					 tmp
					           i
				len = i - tmp = 9 - 4 = 5
				e infatti nella funziona malloco per 5 + 1 cosi ho tutto $HOME\0 che son 6 celle
		 */
		if (check_symbols(str_to_tokenize, &i) > 0)
		{	
			len = i - tmp;
			tmp = i - len;
			if (str_to_tokenize[tmp] == DOUBLE_QUOTE || str_to_tokenize[tmp] == SINGLE_QUOTE)
				len = len + 1;
			if(str_to_tokenize[tmp] == INPUT_REDIRECTION || str_to_tokenize[tmp] == OUTPUT_REDIRECTION)
				len = len + 1;
			if (len == 0)
				len = 1;
			tokens_matrix[t] = create_token(str_to_tokenize, tmp, len);
			t++;
		}
		i++;
	}
	tokens_matrix[t] = NULL;
	return tokens_matrix;
}

