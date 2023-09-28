/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:14:00 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:27:17 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_static_history(int fd_n, int i)
{
	static int	fd = -1;

	if (!i)
		fd = fd_n;
	return (fd);
}

int	ft_get_history(void)
{
	return (ft_static_history(0, 1));
}

void	ft_put_history(int n)
{
	ft_static_history(n, 0);
}

int	ft_static_proccess(int fd_n, int i)
{
	static int	fd = 0;

	if (!i)
		fd = fd_n;
	return (fd);
}

void	ft_put_proccess(int n)
{
	ft_static_proccess(n, 0);
}
