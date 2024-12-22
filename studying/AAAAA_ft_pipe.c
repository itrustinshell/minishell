#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


t_redir *last_specified_redirection(t_redir *redirlist, int type_redirection)
{
    t_redir *ret;
    t_redir *tmp;
    
    if (!redirlist)
        return (NULL);
    tmp = redirlist;

    ret = NULL;
    while(redirlist)
    {
        if (redirlist->type == type_redirection)
            ret = redirlist;
        redirlist = redirlist->next;
    }
    redirlist = tmp;
    return (ret);
}


void ft_openfiles(t_redir *redirlist)
{
    int fd;
    t_redir *tmp;

    tmp = redirlist;
    while(redirlist)
    {
        //TODO: open only if outredir
        //TODO: manage append
        //TODO: manage heredoc
        if (redirlist->type == OUTPUT_REDIRECTION)
            fd = open(redirlist->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0466);
        else if (redirlist->type == APPEND_REDIRECTION)
            fd = open(redirlist->outredir_file, O_APPEND | O_CREAT | O_TRUNC, 0466);
        redirlist = redirlist->next;
    }
    redirlist = tmp;
}

int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) 
{
    pid_t pid;
    int i;
    // ls > a < b > c >> d < e >> f > g | grep u > aa < bb >> cc < dd < ee > ff >> gg
    //rm a b c d e f g aa bb cc dd ee ff gg 
    i = 0;
    while (cmdlist) 
	{
        pid = fork();
        if (pid == CHILD_PID) 
		{ 
			// Processo figlio
            if (i > 0) 
			{
                // Pipe precedente per input
                if (cmdlist->redirlist)
                {
                    printf("redirections for: %s\n", cmdlist->cmd);
                    //ft_openfiles(cmdlist->redirlist);
                    printf("last in_redir: %s\n",last_specified_redirection(cmdlist->redirlist, INPUT_REDIRECTION)->outredir_file);
                    printf("last out_redir: %s\n",last_specified_redirection(cmdlist->redirlist, OUTPUT_REDIRECTION)->outredir_file);
                    printf("last append_redir: %s\n",last_specified_redirection(cmdlist->redirlist, APPEND_REDIRECTION)->outredir_file);
                }
                close(pipematrix[i - 1][WRITE_END]);
                dup2(pipematrix[i - 1][READ_HERE_STDIN], STDIN_FILENO);
                close(pipematrix[i - 1][READ_END]);
            }
            if (cmdlist->next) 
			{ 
				// Pipe corrente per output
              
                if (cmdlist->redirlist)
                {
                    printf("redirections for: %s\n", cmdlist->cmd);
                    if (cmdlist->redirlist)
                        printf("aaa a lista esiste\n");
                    //ft_openfiles(cmdlist->redirlist);
                    if (cmdlist->redirlist)
                        printf("la lista esiste\n");
                    else
                        printf("AAAAAA la lista non esiste");
                    printf("last in_redir: %s\n",last_specified_redirection(cmdlist->redirlist, INPUT_REDIRECTION)->outredir_file);
                    printf("last out_redir: %s\n",last_specified_redirection(cmdlist->redirlist, OUTPUT_REDIRECTION)->outredir_file);
                    printf("last append_redir: %s\n",last_specified_redirection(cmdlist->redirlist, APPEND_REDIRECTION)->outredir_file);

               
                    //TODO: find last out
                    //TODO: find last in
                }
                close(pipematrix[i][READ_END]);
                dup2(pipematrix[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
                close(pipematrix[i][WRITE_END]);
            }
            cmdlist->path = get_cmdpath(cmdlist->cmd);
            execve(cmdlist->path, cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);
        } 
		else if (pid > 0) 
		{ 
			// Processo padre man mano che si avanza chiude le pipe precedentamente aperte.
            if (i > 0) 
			{
                close(pipematrix[i - 1][READ_END]);
                close(pipematrix[i - 1][WRITE_END]);
            }
            cmdlist = cmdlist->next;
            i++;
        } 
		else 
		{
            perror("Errore fork");
            exit(1);
        }
    }
    while ((pid = wait(NULL)) > 0) 
	{
      //  printf("Figlio terminato (PID: %d)\n", pid);
    }
    return 0;
}
