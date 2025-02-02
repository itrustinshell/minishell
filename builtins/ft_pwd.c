/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: largenzi <largenzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 22:33:26 by largenzi          #+#    #+#             */
/*   Updated: 2025/02/02 22:33:29 by largenzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(void)
{
	char	*cwd_buffer;

	cwd_buffer = NULL;
	cwd_buffer = getcwd(NULL, 0);
	printf("%s\n", cwd_buffer);
	free(cwd_buffer);
	return (1);
}
