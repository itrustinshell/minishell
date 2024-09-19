#include "minishell.h"




void append_node(t_token **node, char *str)
{
	return;
}

int main()
{
	char *str_to_test;
	char **matrix;
	int i;
	t_token *token_lst;

	str_to_test = "ciao come	stai      \n";
	token_lst = (t_token *)malloc(sizeof(t_token));
	if (!token_lst)
		write(1,"token_lst not malloced!", 23);
	token_lst->prev = NULL;
	token_lst->next = NULL;
	matrix = ft_split(str_to_test);
	i = 0;
	while (matrix[i])
	{
		if (i == 0)	
			token_lst->str = matrix[i];
		//aaa quando fai il free ?? di matrix
		i++;
	}

	printf("weweweweweweweweweweweweiiiiiii\n");
	i = 0;
	while (matrix[i])
	{
		printf("%d\n",i);
		printf("%s\n", matrix[i]);
		i++;
	}
	return (0);
}

