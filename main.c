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
	t_env		*env;	
	//int there_is_a_pipe;
	//int there_is_a_builtin;

	(void)argc;
	(void)argv;
	(void)envp;
	env = NULL;
	//global_envlist = copy_envp(envp);
	while (1)
	{	
		inputstr = NULL;
		prompt(&inputstr);

		token_matrix = tokenizer(inputstr); // TOKENIZER
		cmdlist = parsing(token_matrix); //PARSING
		executor(cmdlist, &env); //EXECUTOR




		free(inputstr);
		inputstr = NULL;
		ft_freematrix(token_matrix);
	}
	return (0);
}
