
#include "minishell.h"

/*given a char it checks if it is a space or not*/
int	ft_isspace(char char_to_check)
{
	if (char_to_check == SPACE)
		return (IS_SPACE);
	else if(char_to_check == TABULATION)
		return (IS_SPACE);
	else if (char_to_check == NEW_LINE)
		return (IS_SPACE);
	return (IS_NOT_SPACE);
}

/*it fills the matrix elements with appropriate values*/
char **fill_matrix_elements(char *str, char **matrix)
{
	int x;
	int j;
	int k;

	x = 0;
	j = 0;
	while (matrix[x])
	{	
		while (str[j] && ft_isspace(str[j]))
			j++;
		k = 0;
		while (str[j] && !ft_isspace(str[j]))
		{
			matrix[x][k] = str[j];
			k++;
			j++;
		}
		x++;	
	}
	return (matrix);
}

/*given a string, it calculates how many words it is composed of.*/
int number_of_matrix_elements(char *str)
{
	int n_elements;
	int counter_padlock;
	int i;

	n_elements = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		counter_padlock = UNLOCKED;
		while (str[i] && !ft_isspace(str[i]))
		{
			if (counter_padlock == UNLOCKED)
			{
				n_elements += 1;
				counter_padlock = LOCKED;
			}
			i++;
		}	
	}
	return (n_elements);
}

int calculate_element_len(char *str)
{
	static int i;
	int element_len;

	while (str[i] && ft_isspace(str[i]))
		i++;
	element_len = 0;
	while (str[i] && !ft_isspace(str[i]))
	{	
		element_len++;
		i++;
	}
	return (element_len);
}

/*it just allocate memory for matrix element.
 It doesn't fill the elements with values.
 It word_len > 0 means that there is an element to insert*/
char **allocate_memory_for_elements(char *str, char **matrix, int n_elements)
{	
	int x;
	int element_len;

	x = 0;
	while (x < n_elements)
	{	
		element_len = calculate_element_len(str);
		if (element_len > 0)
		{	
			matrix[x] = (char *)calloc((element_len + 1), sizeof(char));
			if (!matrix[x])
				return (NULL);
			matrix[x][element_len] = '\0';
			x++;
		}
		else
			break;
	}
	return (matrix);
}

/*it create just the structure of matrix, with empty elements.
 In other words it allocates al the memory ofr each element.*/
char **build_matrix_structure(int n_elements, char *str)
{
	char **matrix;

	matrix = NULL;
	matrix = (char **)calloc((n_elements + 1), sizeof(char *));
	if (!matrix)
		return (NULL);
	matrix[n_elements] = NULL;
	matrix = allocate_memory_for_elements(str, matrix, n_elements);
	return (matrix);
}

char **ft_split(char *str_to_split)
{
	char **matrix;
	int n_elements;

	if (!str_to_split)
		return (NULL);
	matrix = NULL;
	n_elements = number_of_matrix_elements(str_to_split);
	matrix = build_matrix_structure(n_elements, str_to_split);
	if (!matrix)
		return (NULL); //check for free
	matrix = fill_matrix_elements(str_to_split, matrix);
	if (!matrix)
		return (NULL);
	return (matrix);
}

