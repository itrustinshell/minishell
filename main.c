#include "minishell.h"
#include <signal.h>
#include<readline/history.h>

/*TODO: 
- isspace
- strcmp
- atoi
- isdigit

- nel tentativo di fixare echo $? mi sono accorto che $PATH non stampa tutto il PATH

-aaa:se premo piu volte tab mi fa ls
*/

static int g_signal_received = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = 1;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int sig)
{
    (void)sig;
    g_signal_received = 2;
}

void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // Setup SIGINT (Ctrl-C)
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    // Setup SIGQUIT (Ctrl-\)
    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

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
    
    free(cwdpath);
    free(cwdpath_modified);
    
    if (*inputstr) 
    {
        size_t len = strlen(*inputstr);
        char *with_newline = malloc(len + 2);
        if (!with_newline) 
        {
            free(*inputstr);
            return;
        }
        strcpy(with_newline, *inputstr);
        with_newline[len] = '\0';
        with_newline[len + 1] = '\0';
        free(*inputstr);
        *inputstr = with_newline;
        add_history(*inputstr);
    }
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
	env = copy_envp(envp);
    setup_signals();

    while (1)
    {    
        g_signal_received = 0;
        inputstr = NULL;
        prompt(&inputstr);
        
        if (!inputstr) // Handle Ctrl-D (EOF)
        {
            printf("\nexit\n");
            break;
        }
            
        if (inputstr[0] != '\0' && isspace(inputstr[0]) == 0) // Ignore empty lines
        {
            cmdlist = parse_input(inputstr);
            //printlist(cmdlist);
            if (cmdlist)
            {
                executor(cmdlist, &env, &exit_code);
                if (g_signal_received == 2)  // If SIGQUIT was received
                {
                    printf("Quit (core dumped)\n");
                    exit_code = 131;
                    //free_cmd(cmdlist); tolti a causa di double free
                    //free(inputstr); tolti a causa di double free
                    break;
                }
                //free_cmd(cmdlist); tolti a causa di double free
            }
        }
        free(inputstr); 
        inputstr = NULL;
    }

    if (env)
        ft_freelist(env);
    return (exit_code);
}
