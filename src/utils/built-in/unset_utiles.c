/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:51:16 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/06 14:21:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	is_valid_option(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
		{
			if (!ft_isdigit(str[i]))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	not_valid_iden_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	errno = 1;
}
