#include "minishell.h"

void prompt(char **inputstr)
{
	char *cwdpath;
    char *cwdpath_modified;
    int   len;

	cwdpath = getcwd(NULL, 0);
	len = ft_strlen(cwdpath);

    cwdpath_modified = malloc(len + 3);
    strcpy(cwdpath_modified, cwdpath);
    cwdpath_modified[len] = ' ';
    cwdpath_modified[len + 1] = ':';
    cwdpath_modified[len + 2] = '\0';

    *inputstr = readline(cwdpath_modified);
    if (*inputstr) 
    {
        size_t len = strlen(*inputstr);
        char *with_newline = malloc(len + 2); // +1 per '\n' e +1 per '\0'
        if (!with_newline) 
            free(*inputstr);
        strcpy(with_newline, *inputstr);
        with_newline[len] = '\0'; //ho dovuto aggiungere qui un \0 al posto del \n precedentemente aggiuto. Pecckè a storie iev abbascie. Damn holy nothing.
        with_newline[len + 1] = '\0'; //let's go to see (iamm a vrè)
        free(*inputstr);
        *inputstr = with_newline;
    }
    if (*inputstr)
    {
        add_history(*inputstr);
    }
    else
        printf("weeee\n");
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
		//printf("sono nel main. Dopo executor. e questo è l'exit_code: %d\n", exit_code);
		 free(inputstr);
		// inputstr = NULL;
		ft_freematrix(token_matrix);
	}
	return (0);
}
