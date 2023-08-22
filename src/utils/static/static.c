/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:08:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/14 14:05:22 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_static *ft_static(int modify, t_static *new)
{
	static t_static *s = NULL;
	if (modify == 1)
		s = new;
	return (s);
}

t_static	*ft_get_static(void)
{
	return (ft_static(0, NULL));
}

t_static	*ft_put_static(t_static *new)
{
	return (ft_static(1, new));
}

