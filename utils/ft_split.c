#include "../minishell.h"


int countwords(char *str, char separator)
{
	int n_word;
	int padlock;
	int i;
	//se vuota restiruisci zero
	n_word = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == separator)
			i++;
		padlock = 1;
		while (str[i] && str[i] != separator)
		{
			if (padlock == 1)
			{
				n_word += 1;
				padlock = 0;
			}
			i++;
		}
	}
	return n_word;
}

char **ft_split(char *str, char separator)
{
	int n_words;
	char **matrix;
	int i;

	n_words = countwords(str, separator);
//	printf("number of word: %d\n", n_words);
	matrix = (char **)malloc((n_words + 1) * sizeof(char *));
	if (!matrix)
		return (NULL);
	i = 0;
	int j = 0;
	int k;
	while (i < n_words)
	{
		while (str[j] == separator)
			j++;
		k = 0;
		while (str[j + k] && str[j + k] != separator)
			k++;
		matrix[i] = (char *)malloc((k + 1) * sizeof(char));
		if (!matrix[i])
			ft_free_n_matrix(matrix, i);
		k = 0;
		while (str[j + k] && str[j + k] != separator)
		{
			matrix[i][k] = str[j + k];
			k++;
		}
		j = j + k;
		i++;
	}
	matrix[i] = NULL;
	return matrix;	
}

/*
void test_print_matrix(char **matrix)
{
	int i;


	i = 0;


	while (matrix[i])
	{
		printf("%s\n", matrix[i]);
		i++;
	}
}

int main(void)
{
	char *str;
	char **matrix;

	str = " s:ciao:come:stai:io:sono:un:ragazzo:";	
	matrix = ft_split(str, ':');
	test_print_matrix(matrix);
	return (0);
}
*/
