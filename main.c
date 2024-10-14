#include "minishell.h"


int main(int argc, char **argv)
{
	char *str;
	size_t len;
	char **matrix;
	int i;
	int num_of_nodes;
	t_command *commandlist;
	int **array_pipe;

	str = NULL;
	len = 0;

	getline(&str, &len, stdin);

	matrix = tokenizer(str);
	print_matrix_of_char(matrix);
	if (check_if_there_is_at_least_one_pipe(matrix) == THERE_IS_A_PIPE)
	{
		commandlist = pipe_management(matrix);
		num_of_nodes = number_of_nodes_in_a_list(commandlist);
		array_pipe = generate_array_of_pipes_with_fd(num_of_nodes);
		print_matrix_of_int(array_pipe, (num_of_nodes - 1), 2);
		char *ext_cmd =  find_external_cmd(commandlist[0].cmd);
		execve(ext_cmd, commandlist[0].args, NULL);
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


