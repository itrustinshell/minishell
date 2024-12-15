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
	/*
	mapindications = ft_split(strmaps, '/');
	printf("Here below the map_indications\n");
	i = 0;
	while (mapindications[i])
	{
		printf("%d: %s\n", i, mapindications[i]);
		i++;
	}
	splittedcwd = ft_split(cwdpath, '/');
	i=0;
	printf("Here below, the splittedcwd:\n");
	while (splittedcwd[i])
	{
		printf("%d: %s\n", i, splittedcwd[i]);
		i++;
	}
	i = 0;
	while (mapindications[i])
	{
		while (mapindications[i] && strcmp(mapindications[i], "..") == 0)
		{
			printf("test\n");
			i++;
		}
		printf("weeeee\n");
		if (!mapindications[i])
		{ 
			printf("i got a null\n");
			break;
		}
		else
			i++;		
	}	
	printf("there are %d times ..\n", i);
	*/
	chdir(strmaps);
	char *str;
	str = getcwd(NULL, 0);
	printf("this is new dir:%s\n", str);
}
