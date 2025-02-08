#include "../minishell.h"

char **matrixalloc(int n_tokens)
{
	char **matrix_to_ret;

	matrix_to_ret = NULL;
	matrix_to_ret = (char **)malloc((n_tokens + 1) * sizeof(char *));
	if (!matrix_to_ret)
		return (NULL);
	return (matrix_to_ret);
}
