/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:13:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:18:48 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_get_error(void)
{
	return (*(int *)(ft_get_static())->error->content);
}

void	ft_put_error(int error)
{
	*(int *)(ft_get_static())->error->content = error;
}

t_static	*ft_static(int modify, t_static *new)
{
	static t_static	*s = NULL;

	if (modify == 1)
		s = new;
	return (s);
}

int	ft_get_proccess(void)
{
	return (ft_static_proccess(0, 1));
}
