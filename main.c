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
	char buffer[100];

	str = NULL;
	len = 0;

	getline(&str, &len, stdin);

	matrix = tokenizer(str);
	print_matrix_of_char(matrix);
	if (check_if_there_is_at_least_one_pipe(matrix) == THERE_IS_A_PIPE)
	{
	
		commandlist = pipe_management(matrix);
		
		t_command *current;
		int a;

		printf("i'm printing command list\n");
		current = commandlist;
		a = 0;
		while (current)
		{
			printf("cmd %d: %s\n", a, current->cmd);
			current = current->next;
			a++;
		}

		
		
		num_of_nodes = number_of_nodes_in_a_list(commandlist);
		array_pipe = generate_array_of_pipes_with_fd(num_of_nodes);
		print_matrix_of_int(array_pipe, (num_of_nodes - 1), 2);
		//ora ho un processo figlio
		pid_t pid = fork();
		if (pid == 0)
		{
			printf("yes, i'm the child process\n");
			char *ext_cmd =  find_external_cmd(commandlist->cmd);
			//lavoro sugli stream
			close(array_pipe[0][0]);
			dup2(array_pipe[0][1],1);
			close(array_pipe[0][1]);
			//fine lavoro sugli stream...ora vierrò rediretto lo stdout nella pipe
			
			execve(ext_cmd, commandlist->args, NULL);
			_exit(0);
		}
		pid = fork();
		if(pid == 0)
		{
			close(array_pipe[0][1]);
			printf("I'm the second child  process\n");
			char *ext_cmd_two = find_external_cmd(commandlist->next->cmd);
			printf("i'm passing this command: %s\n", commandlist->next->cmd);
			dup2(array_pipe[0][0], 0);
			close(array_pipe[0][0]);
			execve(ext_cmd_two, commandlist->next->args, NULL);
		//	read(0, buffer, sizeof(buffer));
		//	printf("here the buffer:\n%s\n", buffer);
			_exit(0);
		}
		close(array_pipe[0][0]);
		close(array_pipe[0][1]);
		wait(NULL);
		wait(NULL);
	}	
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
//	free(str);
	return (0);
}
