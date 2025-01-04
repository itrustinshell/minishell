#include "../minishell.h"





//DA IMPLEMENTARE
int ft_cd(char **argv)
{
	char	*cwdpath;
	char	*strmaps;

	strmaps = argv[1];
	cwdpath = getcwd(NULL, 0);
	printf("hei I am in cd funciton and this is the current path:\n%s\n", cwdpath);
	if (strmaps)
	{
		printf("yes strmpas exists\n");
	}
	else
	{
		printf("no, strmaps doesn't exist\n");
		return 0;
	}
	chdir(strmaps);
	char *str;
	str = getcwd(NULL, 0);
	printf("this is new dir:%s\n", str);
	//a questo punto posso dire che se lo strcmp tra str e cwdpath non cambia significa che non ho cambiato cartella e quindi che il path non esiste e bash allora puo generare un messaggo di errore.
	return (1);
}



// int main (int argc, char **argv)
// {
// 	char	*cwdpath;
// 	char	*strmaps;
// 	char	**mapindications;
// 	char	**splittedcwd;
// 	int		i;
// 	char	*finalpath;

// 	strmaps = argv[1];
// 	cwdpath = getcwd(NULL, 0);
// 	printf("hei I am in cd funciton and this is the current path:\n%s\n", cwdpath);
// 	if (strmaps)
// 	{
// 		printf("yes strmpas exists\n");
// 	}
// 	else
// 	{
// 		printf("no, strmaps doesn't exist\n");
// 		return 0;
// 	}
// 	chdir(strmaps);
// 	char *str;
// 	str = getcwd(NULL, 0);
// 	printf("this is new dir:%s\n", str);
// }
