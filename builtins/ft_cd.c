#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

int main (int argc, char **argv)
{
	char	*cwdpath;
	char	*strmaps;
	char	**mapindications;
	char	**splittedcwd;
	int		i;
	char	*finalpath;

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
}
