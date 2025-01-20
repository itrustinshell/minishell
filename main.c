#include "minishell.h"

void prompt(char **inputstr)
{
    //size_t len;
    char *cwdpath;

    //len = 0;
    *inputstr = NULL;
    cwdpath = getcwd(NULL, 0);
    printf("%s💪💪💪: ", cwdpath);
    free(cwdpath);
    *inputstr = readline(""); // Usando readline invece di getline per consistenza
    //if (*inputstr)
    //    add_history(*inputstr);
}

int main(int argc, char **argv, char **envp)
{
    char    *inputstr;
    t_cmd   *cmdlist;    
    t_env   *env;
    int     exit_code;    

    exit_code = 0;
    (void)argc;
    (void)argv;
    env = NULL;
    //env = copy_envp(envp);

    while (1)
    {    
        inputstr = NULL;
        prompt(&inputstr);
        
        if (!inputstr) // Gestione di Ctrl+D (EOF)
            break;
            
        if (inputstr[0] != '\0') // Ignora linee vuote
        {
            cmdlist = parse_input(inputstr);
            if (cmdlist)
            {
                executor(cmdlist, &env, &exit_code);
                free_cmd(cmdlist);
            }
        }
        
        free(inputstr);
        inputstr = NULL;
    }

    // Cleanup finale
    if (env)
        ft_freelist(env);
    printf("exit\n");
    return (exit_code);
}