	char **l_envp;
	char test_g_envp[3][8] = {"aaa=123", "bbb=123", "ccc=123"};
	char test_l_envp[3][8] = {"sss=123", "ttt=123", "rrr=123"};

	l_envp = NULL;
	g_envp = test_create_envp(test_g_envp);
	l_envp = test_create_envp(test_l_envp);

	print_matrix_of_char(g_envp);
	print_matrix_of_char(l_envp);




char **convert_list_to_matrix(t_env *envlist)
{
	int lenlist;
	t_env *tmp_envlist;
	char **env_matrix;
	int len_name;
	int len_value;
	int i;
	int n;
	int m;

	tmp_envlist = envlist;
	printf("inizio a convertire\n");
	lenlist = envlistlen(tmp_envlist);
	printf("la lista si compone di: %d nodi.\n", lenlist);

	env_matrix = (char **)malloc((lenlist + 1) * sizeof(char *));

	i = 0;
	while (tmp_envlist)
	{

		//calcola la lungezza di name
		len_name = ft_strlen(tmp_envlist->name);
		len_value = ft_strlen(tmp_envlist->value);
		env_matrix[i] = (char *)malloc((len_name + len_value + 1 + 1) * sizeof(char));
		n = 0;
		m = 0;
		while(tmp_envlist->name[n])
		{
			env_matrix[i][m] = tmp_envlist->name[n];
			m++;
			n++;
		}
		env_matrix[i][m] = '=';
		m++;
		n = 0;
		while(tmp_envlist->value[n])
		{
			env_matrix[i][m] = tmp_envlist->value[n];
			m++;
			n++;
		}
		env_matrix[i][m] = '\0';
		i++;
		tmp_envlist = tmp_envlist->next;
	}
	env_matrix[i] = NULL;
	return (env_matrix);
}