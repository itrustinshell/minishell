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
	t_cmd	*cmdlist;	
	t_env		*env;
	int		exit_code;	

	exit_code = 0;
	(void)argc;
	(void)argv;
	(void)envp;
	env = NULL;
	//env = copy_env(envp);
	while (1)
	{	
		inputstr = NULL;
		prompt(&inputstr);
		token_matrix = tokenizer(inputstr); //TOKENIZER
		cmdlist = parsing(token_matrix); //PARSING
		executor(cmdlist, &env, &exit_code); //EXECUTOR
		printf("sono nel main. Dopo executor. e questo è l'exit_code: %d\n", exit_code);
		free(inputstr);
		inputstr = NULL;
		ft_freematrix(token_matrix);
	}
	return (0);
}
