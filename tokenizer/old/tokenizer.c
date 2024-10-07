
#include "minishell.h"

/*it create just the structure of matrix, with empty elements.
 In other words it allocates al the memory ofr each element.*/
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


int main(int argc, char **argv)
{
	char *str;
	size_t len;
	char **matrix;

	str = NULL;
	len = 0;

	getline(&str, &len, stdin);

	matrix = tokenizer(str);
	int i = 0;
		

	while(matrix[i])
	{
		printf("%s\n", matrix[i]);
		i++;
	}
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	free(str);
}


