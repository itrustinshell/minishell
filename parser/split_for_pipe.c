#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "../minishell.h"


static int	my_count_words( char *str, char c)
{
	int	i;
	int	words;
	//printf("but what....\n");

	i = 0;
	words = 0;
	while (str[i])
	{
        if (str[i] == '"')
        {
            i++; //tras nta sti c..z e virgolett
            while (str[i] != '"') //cicl fin a che nun ce sta nata virgolett (se non mettono la seconda virgoletta....gestire)
                i++;
            // è sciut ro cicl. A truat a virgolett!
        }
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0')) //si si asciut ro cicl, mo stai ncopp a siconda virgolett.
			words++;
		i++;
	}
	return (words);
}

static void	my_new_string( char *str, int start, int end, char *dest)
{
	int	i;

	i = 0;
	while (i < end)
	{
		dest[i] = str[start + i];
		i++;
	}
	dest[i] = '\0';
}

static void	my_aux(char **arr,  char *str, char sep, int n_words)
{
	int	i;
	int	j;
	int	z;
	//int x;
  
	i = 0;
	z = 0;

	while (str[i] && z <= n_words)
	{
		if (str[i] == sep)
			i++;
		else
		{
			j = 0;
			while (str[i + j] && str[i + j] != sep)
            {
                if (str[i + j] == '"')
                {
					j++;
					printf("sono entrato\n");
                    while (str[i + j] != '"')
                        j++;
                }
                j++;
            }
			//printf("ecco la lunghezza della parola: %d\n", j);
            arr[z] = (char *)malloc(sizeof(char) * (j + 1));
			if (!arr[z])
				return ;
			my_new_string(str, i, j, arr[z]);

			// x = 0;
			// while (x < j) 
			// {
			// 	arr[z][x] = str[i + x];
			// 	x++;

			// }
			//printf("ecco la nuov stringa: %s\n", arr[z]);
			z++;
			i += j;
		}
	}
}

char	**ft_split_for_pipe( char *str, char c)
{
	char	**arr;
	int		n_words;

	if (!str)
		return (NULL);
	n_words = my_count_words(str, c);
	arr = ft_calloc(n_words + 1, sizeof(char *));
	arr[n_words] = NULL;
	if (!arr)
		return (NULL);
	my_aux(arr, str, c, n_words);
	return (arr);
}

