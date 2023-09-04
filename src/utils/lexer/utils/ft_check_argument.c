/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_argument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:12:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:12:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//			ft_check_argument
//		return values:
//		1 if is a valid argument 
//		0 if is a empty argument
//		-1 if is a invalid argument

int	ft_check_argument(char *str)
{
	int	len;

	len = ft_get_next_command(str);
	if (len == -4)
		return (0);
	while (len > 0)
	{
		str += len;
		len = ft_get_next_command(str);
	}
	if (len == -1)
		return (-1);
	return (1);
}
