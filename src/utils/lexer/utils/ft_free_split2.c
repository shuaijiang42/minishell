/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:14:38 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:15:14 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_free_split_2(char ***split)
{
	int i;

	i = 0;
	while (split[0][i])
	{
		if (split[0][i])
			ft_free((void *)&split[0][i]);
		i++;
	}
	ft_free((void *)&split[0]);
	*split = NULL;
	return (NULL);
}
