
#include "minishell.h"

/*it create just the structure of matrix, with empty elements.
 In other words it allocates al the memory ofr each element.*/
char **matrixalloc(int n_tokens, char *str)
{
	char **matrix_to_ret;

	matrix_to_ret = NULL;
	matrix_to_ret = (char **)malloc((n_tokens + 1) * sizeof(char *));
	if (!matrix_to_ret)
		return (NULL);
	matrix_to_ret[n_tokens] = NULL;
	return (matrix_to_ret);
}

char **tokenizer(char *str_to_tokenize)
{
	char **tokens_matrix;
	int n_tokens;

	if (!str_to_tokenize)
		return (NULL);
	tokens_matrix = NULL;
	n_tokens = get_num_of_tokens(str_to_tokenize);
	tokens_matrix = matrixalloc(n_tokens, str_to_tokenize);
	if (!tokens_matrix)
		return (NULL);
/*	
	tokens_matrix = fill_tokensmatrix(str_to_split, matrix);
	if (!matrix)
		return (NULL);
*/	
	printf("number of tokens: %d\n", n_tokens);
	return (tokens_matrix);
}


int main(int argc, char **argv)
{
	char *str;
	size_t len;
	char **matrix;

	str = NULL;
	len = 0;

	getline(&str, &len, stdin);


	matrix = tokenizer(str);
}


