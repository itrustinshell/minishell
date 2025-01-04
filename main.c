#include "minishell.h"

void prompt(char **inputstr)
{
	size_t len;
	char *cwdpath;

	len = 0;
	*inputstr = NULL;
	cwdpath = getcwd(NULL, 0);
	printf("%s💪💪💪: ", cwdpath);
	getline(inputstr, &len, stdin); //free di inputstr fatto!
}

int main(int argc, char **argv, char **envp)
{
	char		*inputstr;
	char		**matrix;
	t_command	*cmdlist;
	//int			cmdlist_len;
	
	t_env		*global_envlist;	

	(void)argc;
	(void)argv;
	(void)envp;
	//global_envlist = copy_envp(envp);
	global_envlist = NULL;
	matrix = NULL;
	//cmdlist_len = 0;
	while (1)
	{	
		inputstr = NULL;
		prompt(&inputstr);
		matrix = tokenizer(inputstr);
		//print_matrix_of_char(matrix);
		if (check_pipe_symbol(matrix) == THERE_IS_A_PIPE)
		{
			cmdlist = commandlist_for_pipe(matrix);
			execute_multiple_cmd(cmdlist, global_envlist);
		}
		else if(!check_builtin_for_singlecmd(matrix, &global_envlist, inputstr))
			execute_single_cmd(matrix);
		free(inputstr);
		inputstr = NULL;
		ft_freematrix(matrix);

	}
	return (0);
}
