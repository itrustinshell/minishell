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
    {
        printf("Pay attention! you passed a null list to last_specified_redirection function\n");
        return (NULL);
    }
    tmp = redirlist;

    ret = NULL;
    while(tmp)
    {
       // printf("red: %s\n", tmp->outredir_file);
        if (tmp->type == type_redirection)
        {
           // printf("i found same redir\n");
            ret = tmp;
         //   if (ret)
            //    printf("ret esisteeeee\n");
            //else
              //  printf("ret non esisteee\n");
        }
        //printf("now i go to tne next\n");
        tmp = tmp->next;
    }
    if (!tmp)
      //  printf("hei it has finished\n");
 
    tmp = redirlist;
   
    // if (ret)
    //     printf("bbboooooooo\n");
    // else
    //     printf("ret é NULLO!\n");
    return (ret);
}

void ft_openfiles(t_redir *redirlist)
{
    //int fd;
    t_redir *tmp;

    tmp = redirlist;
    while(tmp)
    {
        //TODO: open only if outredir
        //TODO: manage append
        //TODO: manage heredoc
        if (tmp->type == OUTPUT_REDIRECTION)
            tmp->fd = open(tmp->outredir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        else if (tmp->type == APPEND_REDIRECTION)
            tmp->fd = open(tmp->outredir_file, O_APPEND | O_CREAT | O_TRUNC, 0666);
        tmp = tmp->next;
    }
    tmp = redirlist;
}

int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) 
{
    pid_t       pid;
    int         i;
    t_command   *tmp_cmdlist;
    t_redir     *tmp_redirlist;
    t_redir     *ret;
    // ls > a < b > c >> d < e >> f > g | grep u > aa < bb >> cc < dd < ee > ff >> gg
    //rm a b c d e f g aa bb cc dd ee ff gg 
    tmp_cmdlist = cmdlist;
    
    i = 0;
    while (tmp_cmdlist) 
	{
        tmp_redirlist = tmp_cmdlist->redirlist;
        pid = fork();
        if (pid == CHILD_PID) 
		{ 
			// Processo figlio
            if (i > 0) 
			{
                // Pipe precedente per input
                ft_openfiles(tmp_redirlist);
                if (tmp_redirlist)
                {
                    ret = last_specified_redirection(tmp_redirlist, INPUT_REDIRECTION);
                    if (ret)
                        printf("last input_redir: %s\n", ret->outredir_file);
                    ret = last_specified_redirection(tmp_redirlist, OUTPUT_REDIRECTION);
                    if (ret)
                        printf("last out_redir: %s\n", ret->outredir_file);
                    ret = last_specified_redirection(tmp_redirlist, APPEND_REDIRECTION);
                    if (ret)
                        printf("last out_redir: %s\n", ret->outredir_file);            
                }
                close(pipematrix[i - 1][WRITE_END]);
                dup2(pipematrix[i - 1][READ_HERE_STDIN], STDIN_FILENO);
                close(pipematrix[i - 1][READ_END]);
            }
            if (tmp_cmdlist->next) 
			{ 
				// Pipe corrente per output
                ft_openfiles(tmp_redirlist);
                if (tmp_redirlist)
                {
                    ret = last_specified_redirection(tmp_redirlist, INPUT_REDIRECTION);
                    if (ret)
                        printf("last input_redir: %s\n", ret->outredir_file);
                    ret = last_specified_redirection(tmp_redirlist, OUTPUT_REDIRECTION);
                    if (ret)
                        dup2(ret->fd, STDOUT_FILENO);
                    ret = last_specified_redirection(tmp_redirlist, APPEND_REDIRECTION);
                    if (ret)
                        printf("last out_redir: %s\n", ret->outredir_file);
                }
                else
                {
                    close(pipematrix[i][READ_END]);
                    dup2(pipematrix[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
                    close(pipematrix[i][WRITE_END]);
                }
            }
            tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
            execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
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
            tmp_cmdlist = tmp_cmdlist->next;
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
