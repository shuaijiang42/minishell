/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 18:47:39 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/04 19:18:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	result;

	result = 0;
	if (!lst)
		return (0);
	if (!lst -> next)
		return (1);
	else
	{
		while (lst)
		{
			result++;
			lst = lst -> next;
		}
	}
	return (result);
}
