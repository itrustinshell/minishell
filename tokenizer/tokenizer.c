
#include "../minishell.h"

char **tokenizer(char *str_to_tokenize)
{
	char **tokens_matrix;
	int n_tokens;
	int i;

	if (!str_to_tokenize)
		return (NULL);
	tokens_matrix = NULL;
	n_tokens = get_num_of_tokens(str_to_tokenize);
	if (n_tokens < 0)
		return (NULL);
	tokens_matrix = create_tokenmatrix(str_to_tokenize, n_tokens);
	if (tokens_matrix == NULL)
		return (NULL);
	return (tokens_matrix);
}

