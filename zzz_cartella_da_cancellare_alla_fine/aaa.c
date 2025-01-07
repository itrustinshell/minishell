


int	ft_echo(int argc, char **argv, int *exit_code)
{
	int	i;
	int j;

	if (strcmp(argv[1], "-n") == 0)
	{
		i = 2;

		j = 0;
		if (argv[2][j] == '$')
			j++;
		if (argv[2][j] == '?')
			j++;
		while (argv[2][j])
		{
			if (argv[2][j] == '\n' || argv[2][j] == ' ')
			{
				argv[2][j] = '\0';
				break;
			}
			j++;
		}
		if (strcmp(argv[2], "$?") == 0)
		{
			printf("%d", *exit_code);
			i++;
		}


		while (argv[i] && i < argc - 1)
		{
			printf(" %s", argv[i]);
			i++;
		}
		if (argv[i])
			printf(" %s", argv[i]);
	}
	else
	{
		i = 1;
		
		j = 0;
		if (argv[1][j] == '$')
			j++;
		if (argv[1][j] == '?')
			j++;
		while (argv[1][j])
		{
			if (argv[1][j] == '\n' || argv[1][j] == ' ')
			{
				argv[1][j] = '\0';
				break;
			}
			j++;
		}
		
		if (strcmp(argv[1], "$?") == 0)
		{
			printf("%d ", *exit_code);
			i++;
		}
		else
			//printf("this is arg[1]: %s.", argv[1]);
		while (argv[i] && i < argc - 1)
		{
			printf("%s ", argv[i]);
			i++;
		}
		if (argv[i])
			printf("%s\n", argv[i]);
		else
			printf("\n");
	}
	return (1);
}


void cmd_before_single_outredirection(char **matrix)
{
	t_cmd	*cmd;
	int			i;
	int			fd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	
	cmd->cmd = strdup(matrix[0]);
	cmd->path = get_cmdpath(cmd->cmd);
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
		i++;
	cmd->args = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (matrix[i][0] != OUTPUT_REDIRECTION)
	{
		cmd->args[i] = strdup(matrix[i]);
		i++;
	}
	cmd->args[i] = NULL;
	i++;
	fd = open(matrix[i], O_WRONLY | O_CREAT | O_TRUNC, 0466); //TODO aprire il file dato in path assoluto e relativo
	
	int pid = fork();

	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execve(cmd->path, cmd->args, NULL);
	}
	else
	{
		close(fd);
		wait(NULL);
	}		
}

