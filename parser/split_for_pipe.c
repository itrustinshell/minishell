/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_for_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:05:51 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/14 13:05:57 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <strings.h>
#include "../minishell.h"

static int	my_count_words( char *str, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (str[i])
	{
        if (str[i] == '"')
        {
            i++;
            while (str[i] != '"')
                i++;
        }
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
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
            arr[z] = (char *)malloc(sizeof(char) * (j + 1));
			if (!arr[z])
				return ;
			my_new_string(str, i, j, arr[z]);
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

