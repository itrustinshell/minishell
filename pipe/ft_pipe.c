#include "../minishell.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


char	*get_cmdpath(char *cmd)
{
	char	*cmdpath;

	if (strcmp(cmd, "echo") == 0)
		cmdpath = "/home/ubuntu/mine/minishell/builtins/myecho";
	else if (strcmp(cmd, "pwd") == 0)
		cmdpath = "/home/ubuntu/mine/minishell/builtins/mypwd";
	else if (strcmp(cmd, "cd") == 0)
		cmdpath = "/home/ubuntu/mine/minishell/builtins/mycd";
	else 
		cmdpath = find_external_cmd(cmd);

	return (cmdpath);
}

int general_pipe_management(t_command *cmdlist, int cmdlist_len, int ** pipesarray)
{
	int			i;
	pid_t		pid;
	char		*ext_cmd;

	i = 0;	
	while (cmdlist)
	{
		if (i == 0)
		{
			pid = fork();
			if (pid == CHILD_PID0) //child
			{
				close(pipesarray[i][READ_HERE_STDIN]);
				//	nella primissima pipe io non devo leggere niente..è vuota.
				//	 Invece devo riempirla. Devo SCRIVERCI qualcosa.
				//	 Mi servirà pertanto la sua estremità di scrittura.
				//	 In questa estremità di scrittura ci scriverò quello che
				//	 normalmente io vedrei stampato a schermo, ovvero l'OUTPUT.
				//	 Fai cosi: quando leggi dup2 parti da destra e dci:
				//	 dove lo mando STDOUT? sul WRITE.
				//	 In caso di STDIN la domanda cambia: DA DOVE lo prendo lo STDIN dal READ.
					 
				dup2(pipesarray[i][WRITE_HERE_STDOUT], STDOUT_FILENO); //ora stdout non viene mandato a schermo ma viene scritto nella pipe
				close(pipesarray[i][WRITE_HERE_STDOUT]);
					//ora nel child il READ e il WRITE della prima pipe sono chiusi
					//ma non sonon chiusi nel parent. Il parent ha ancora entrambe le
					//estremità della prima pipe aperte.
				cmdlist->path = get_cmdpath(cmdlist->cmd);
				execve(cmdlist->path, cmdlist->args, NULL); 
				return(1);
			}
			else if (pid > CHILD_PID0) //parent
			{
					//non faccio chiudere niente al pipe....vado al processo successivo 
					//che si precoccupa di leggere 
					close(pipesarray[i][WRITE_HERE_STDOUT]);
			close(pipesarray[i][READ_HERE_STDIN]);
		cmdlist = cmdlist->next;
				i++;
				if (cmdlist_len == 2)
					break;
				continue;
			}
		}
			//Processi intermedi
		pid = fork();
		if (pid == CHILD_PID0) //child
		{	
			//	qui sono dal secondo processo ...ed eventualmente quelli successivi...non è l'ultimo
			//	 Ora qui se sono al secondo, allora dal padre ho ereditato la pipe precedente che
			//	 è stata chiusa nel child precedente ma non nel parent appunto.
			//	 Quindi ora grazie al parent, avendo fatto il fork me la ritrovo nel nuovo child.
			//	 E posso quindi usarla per accedere ai volori che prima ho scritt.
			//	 Quindi ora non devo scrivere nella precedente pipe ma devo leggere da essa.
			//	 Quindi anzitutto posso chiudere l'estremità di scrittura della precedente pipe.
			//	 Bene, ora devo leggere da quella precedente pipe quello che avevo scritto.
			//	 E quello che leggerò sarà il mio input per il comando successivo.
			//	 Quindi l'input non lo trovo sullo STDIN, ma nella estremità di lettura della
			//	 precedente pipe
				 
			close(pipesarray[i - 1][WRITE_HERE_STDOUT]); //questo comando si trova tra due pipe...quindi si riferisce alla pipe precedente per leggere input e a quella successiva per scrivere output
			dup2(pipesarray[i - 1][READ_HERE_STDIN], STDIN_FILENO); //AAA i demità di lettura
			close(pipesarray[i - 1][READ_HERE_STDIN]);
			if (cmdlist->next) // S dso la pipe successiva
			{
				close(pipesarray[i][READ_HERE_STDIN]);
				dup2(pipesarray[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
				close(pipesarray[i][WRITE_HERE_STDOUT]);
			}
			cmdlist->path = get_cmdpath(cmdlist->cmd);
			execve(cmdlist->path, cmdlist->args, NULL);
			return(1);
		}
		else if (pid > CHILD_PID0) //parent
		{				close(pipesarray[i][WRITE_HERE_STDOUT]);
			close(pipesarray[i][READ_HERE_STDIN]);

			cmdlist = cmdlist->next;
			i++;
		}
	}
		// Ultimo processo
	pid = fork();
	if (pid == CHILD_PID0) //child
	{	
		close(pipesarray[i - 1][WRITE_HERE_STDOUT]); 
		dup2(pipesarray[i - 1][READ_HERE_STDIN], STDIN_FILENO); 
		close(pipesarray[i - 1][READ_HERE_STDIN]);  
		cmdlist->path = get_cmdpath(cmdlist->cmd);
		execve(cmdlist->path, cmdlist->args, NULL);
		return(1);
	}
	else if (pid > CHILD_PID0) //parent
	{			close(pipesarray[i][WRITE_HERE_STDOUT]);
			close(pipesarray[i][READ_HERE_STDIN]);

	/*	i = 0;
		while(i < (cmdlist_len -1 ))
		{
			close(pipesarray[i][WRITE_HERE_STDOUT]);
			close(pipesarray[i][READ_HERE_STDIN]);
			i++;
		}*/
		i = 0;
		while(i < (cmdlist_len ))
		{
			wait(NULL);	
			i++;
		}
	}
	return (1);
}

/*
char *get_command_path(char *cmd) {
    if (strcmp(cmd, "echo") == 0)
        return "/home/ubuntu/mine/minishell/builtins/myecho";
    if (strcmp(cmd, "pwd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mypwd";
    if (strcmp(cmd, "cd") == 0)
        return "/home/ubuntu/mine/minishell/builtins/mycd";
    return find_external_cmd(cmd);
}

int general_pipe_management(t_command *cmdlist, int cmdlist_len, int **pipesarray) {
    int i = 0;
    pid_t pid;
    char *ext_cmd;

    while (cmdlist) {
        pid = fork();
        if (pid == 0) { // Processo figlio
            if (i > 0) { // Pipe precedente per input
                close(pipesarray[i - 1][WRITE_HERE_STDOUT]);
                dup2(pipesarray[i - 1][READ_HERE_STDIN], STDIN_FILENO);
                close(pipesarray[i - 1][READ_HERE_STDIN]);
            }
            if (cmdlist->next) { // Pipe corrente per output
                close(pipesarray[i][READ_HERE_STDIN]);
                dup2(pipesarray[i][WRITE_HERE_STDOUT], STDOUT_FILENO);
                close(pipesarray[i][WRITE_HERE_STDOUT]);
            }
            ext_cmd = get_command_path(cmdlist->cmd);
            execve(ext_cmd, cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);
        } else if (pid > 0) { // Processo padre
            if (i > 0) {
                close(pipesarray[i - 1][READ_HERE_STDIN]);
                close(pipesarray[i - 1][WRITE_HERE_STDOUT]);
            }
            cmdlist = cmdlist->next;
            i++;
        } else {
            perror("Errore fork");
            exit(1);
        }
    }

    while ((pid = wait(NULL)) > 0) {
        printf("Figlio terminato (PID: %d)\n", pid);
    }

    return 0;
}
*/
