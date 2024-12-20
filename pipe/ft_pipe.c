#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


t_redir *last_inputredirection(t_redir *redirlist)
{
    t_redir *ret;

    ret = NULL;
    if (!redirlist)
        return (NULL);
    while(redirlist)
    {
        if (redirlist->type == OUTPUT_REDIRECTION)
            ret = redirlist;
        redirlist = redirlist->next;
    }
    return (redirlist);
}



int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) {
    pid_t pid;


int i = 0;
int m = 0;
int fd;
// while (cmdlist)
// {
//     printf("ecco il comando: %s\n", cmdlist->cmd);
//     if (cmdlist->args)
//     {
//         printf("ecco gli argomenti di %s:\n", cmdlist->cmd);
//         m = -1;
//         while(cmdlist->args[++m])
//             printf("args: %s\n", cmdlist->args[m]);
//     }
//     if (cmdlist->redirlist)
//         printf("ecco le redirections di %s:\n", cmdlist->cmd);
//     while (cmdlist->redirlist)
//     {
//         printf("redir: %s\n", cmdlist->redirlist->outredir_file);
//         cmdlist->redirlist = cmdlist->redirlist->next;
//     } 
//     cmdlist = cmdlist->next;
// }

if (cmdlist)
{

}
    printf("la lista esiste\n");


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
                    printf("ci sono redirection da gestire prima delle pipe\n");
                    m = 0;
                    while(cmdlist->redirlist)
                    {
                        m++;
                        cmdlist->redirlist = cmdlist->redirlist->next;
                    }
                    printf("ho trovato %d redirection\n", m);
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
                    printf("ci sono redirection da gestire prima delle pipe\n");
                    m = 0;
                    cmdlist->redirlist->head = cmdlist->redirlist;
                    while(cmdlist->redirlist)
                    {
                        //TODO: open only if outredir
                        //TODO: manage append
                        //TODO: manage heredoc
                        fd = open(cmdlist->redirlist->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0466);
                        m++;
                        cmdlist->redirlist = cmdlist->redirlist->next;
                    }
                    cmdlist->redirlist = cmdlist->redirlist->head;
                    //TODO: find last out
                    //TODO: find last in
                    printf("ho trovato %d redirection\n", m);
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
