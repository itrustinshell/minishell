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
	char		**token_matrix;
	t_command	*cmdlist;	
	t_env		*global_envlist;	
	int there_is_a_pipe;
	int there_is_a_builtin;

	(void)argc;
	(void)argv;
	(void)envp;
	global_envlist = NULL;
	//global_envlist = copy_envp(envp);
	while (1)
	{	
		inputstr = NULL;
		prompt(&inputstr);
		token_matrix = tokenizer(inputstr); // TOKENIZER
		//print_matrix_of_char(matrix);

		cmdlist = parsing(token_matrix); //PARSING
		
		there_is_a_pipe = check_pipe_symbol(token_matrix);
		there_is_a_builtin = check_builtin_for_singlecmd(token_matrix, &global_envlist, inputstr);
	
		executor(cmdlist, global_envlist, there_is_a_builtin, there_is_a_pipe); //EXECUTOR

		free(inputstr);
		inputstr = NULL;
		ft_freematrix(token_matrix);
	}
	return (0);
}
