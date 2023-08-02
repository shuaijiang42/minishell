/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 12:23:50 by shujiang          #+#    #+#             */
/*   Updated: 2023/05/06 17:59:36 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
	}
	else if (n < 0)
	{
		c = '-';
		write (fd, &c, 1);
		ft_putnbr_fd(-n, fd);
	}	
	if (n >= 0 && n <= 9)
	{
		c = n + 48;
		write (fd, &c, 1);
	}
	if (n > 9)
	{
		c = n + 48;
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(n % 10 + 48, fd);
	}
}

/* int	main()
{
	ft_putnbr_fd(42, 1);
	return (0);
} */