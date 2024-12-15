


int general_pipe_management(t_command *commandlist, int cmdlist_len, int ** pipesarray)
{
	int			i;
	pid_t		pid;
	char		*ext_cmd;

	char *buff_pwd = NULL;



		i = 0;	
		while (commandlist->next)
		{
			if (i == 0)
			{
				pid = fork();
				if (pid == CHILD_PID0) //child
				{
					if (strcmp(commandlist->cmd, "echo") == 0)
						ext_cmd = "/home/ubuntu/mine/minishell/builtins/myecho";
					else if (strcmp(commandlist->cmd, "pwd") == 0)
						ext_cmd = "/home/ubuntu/mine/minishell/builtins/mypwd";
					else if (strcmp(commandlist->cmd, "cd") == 0)
						ext_cmd = "/home/ubuntu/mine/minishell/builtins/mycd";
					else 
						ext_cmd = find_external_cmd(commandlist->cmd);
				//	close(pipesarray[i][READ_END]);
				//	nella primissima pipe io non devo leggere niente..è vuota.
				//	 Invece devo riempirla. Devo SCRIVERCI qualcosa.
				//	 Mi servirà pertanto la sua estremità di scrittura.
				//	 In questa estremità di scrittura ci scriverò quello che
				//	 normalmente io vedrei stampato a schermo, ovvero l'OUTPUT.
				//	 Fai cosi: quando leggi dup2 parti da destra e dci:
				//	 dove lo mando STDOUT? sul WRITE.
				//	 In caso di STDIN la domanda cambia: DA DOVE lo prendo lo STDIN dal READ.
					 
					dup2(pipesarray[i][WRITE_END], STDOUT_FILENO); //ora stdout non viene mandato a schermo ma viene scritto nella pipe
					close(pipesarray[i][WRITE_END]);
					//ora nel child il READ e il WRITE della prima pipe sono chiusi
					//ma non sonon chiusi nel parent. Il parent ha ancora entrambe le
					//estremità della prima pipe aperte.
					execve(ext_cmd, commandlist->args, NULL); 
					return(1);
				}
				else if (pid > CHILD_PID0) //parent
				{
					//non faccio chiudere niente al pipe....vado al processo successivo 
					//che si precoccupa di leggere 
					commandlist = commandlist->next;
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
				 
				
				close(pipesarray[i - 1][WRITE_END]);
				dup2(pipesarray[i - 1][READ_END], STDIN_FILENO); //AAA i demità di lettura
				close(pipesarray[i - 1][READ_END]);
				if (commandlist->next) // S dso la pipe successiva
				{
					close(pipesarray[i][READ_END]);
					dup2(pipesarray[i][WRITE_END], STDOUT_FILENO);
					close(pipesarray[i][WRITE_END]);
				}
				if (strcmp(commandlist->cmd, "echo") == 0)
					ext_cmd = "/home/ubuntu/mine/minishell/builtins/myecho";
				else if (strcmp(commandlist->cmd, "pwd") == 0)
					ext_cmd = "/home/ubuntu/mine/minishell/builtins/mypwd";
				else if (strcmp(commandlist->cmd, "cd") == 0)
						ext_cmd = "/home/ubuntu/mine/minishell/builtins/mycd";
				else
					ext_cmd = find_external_cmd(commandlist->cmd);
				execve(ext_cmd, commandlist->args, NULL);
				return(1);
			}
			else if (pid > CHILD_PID0) //parent
			{	
				commandlist = commandlist->next;
				i++;
			}
		}
		// Ultimo processo
		pid = fork();
		if (pid == CHILD_PID0) //child
		{	
			if (strcmp(commandlist->cmd, "echo") == 0) 
				ext_cmd = "/home/ubuntu/mine/minishell/builtins/myecho";
			else if (strcmp(commandlist->cmd, "pwd") == 0)
				ext_cmd = "/home/ubuntu/mine/minishell/builtins/mypwd";
			else if (strcmp(commandlist->cmd, "cd") == 0)
				ext_cmd = "/home/ubuntu/mine/minishell/builtins/mycd";
			else
				ext_cmd = find_external_cmd(commandlist->cmd);
			close(pipesarray[i - 1][WRITE_END]); 
			dup2(pipesarray[i - 1][READ_END], STDIN_FILENO); 
			close(pipesarray[i - 1][READ_END]); 
			execve(ext_cmd, commandlist->args, NULL); 
			return(1);
		}
		else if (pid > CHILD_PID0) //parent
		{

			i = 0;
			while(i < (cmdlist_len -1 ))
			{
				close(pipesarray[i][WRITE_END]);
				close(pipesarray[i][READ_END]);
				i++;
			}

			i = 0;
			while(i < (cmdlist_len ))
			{
				wait(NULL);	
				i++;
			
			}


		}
		return (1);
}



