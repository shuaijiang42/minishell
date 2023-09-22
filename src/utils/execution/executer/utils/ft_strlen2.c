/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:20:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while(str[i])
		i++;
	return (i);
}
