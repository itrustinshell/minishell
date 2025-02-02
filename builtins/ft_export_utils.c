/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:38:27 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:51:22 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ends_with_equal_sign(char *str)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (str[i])
		i++;
	i--;
	if (str[i] == '=')
		ret = 1;
	return (ret);
}

int	there_is_equal_before_end(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	exportcheck(char **matrix)
{
	int	len;

	len = matrixlen(matrix);
	if (len > 3)
		return (printf("ci sono piu di 3 pezzi con l'export\n"), 0);
	if (!matrix[1])
		return (printf("non c'è il pezzo con l'uguale\n"), 0);
	if (!ends_with_equal_sign(matrix[1]))
	{
		if (!there_is_equal_before_end(matrix[1]))
			return (0);
		if (len >= 3)
			return (printf("ci sono piu di 3 pezzi con l'export\n"), 0);
		return (printf("ok\n"), 1);
	}
	if (len > 3)
		return (printf("troppi argomenti\n"), 0);
	return (printf("ok solo con '='\n"), 2);
}
