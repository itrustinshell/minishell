#include "../minishell.h"



t_env *copy_envp(char **envp)
{
	t_env	*envlist;
	t_env	*envnode;
	int		i;
	int		lenmatrix;
	
	envnode = NULL;
	envlist = NULL;
	lenmatrix = matrixlen(envp);
	printf("il numero di variabili individuate è: %d\n", lenmatrix);
	i = 0;
	while(envp[i])
	{
		envnode = create_lenvnode(envp[i]);
		listappend_envnode(envnode, &envlist);
		i++;
	}
	return (envlist);
}