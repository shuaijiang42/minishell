/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:24:48 by shujiang          #+#    #+#             */
/*   Updated: 2023/04/20 11:37:43 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_print_char(char c)
{
	return (write (1, &c, 1));
}

int	ft_print_str(char *str)
{
	if (str == NULL)
		return (write (1, "(null)", 6));
	else
		return (write (1, str, ft_strlen(str)));
}

void	ft_print_int(int n, int *print_len)
{
	char	c;

	if (n == -2147483648)
	{
		(*print_len) += write (1, "-2147483648", 11);
		return ;
	}	
	else if (n < 0)
	{
		c = '-';
		(*print_len) += write (1, &c, 1);
		n *= -1;
	}	
	if (n < 10)
	{
		c = n + 48;
		(*print_len) += write (1, &c, 1);
	}
	else if (n >= 10)
	{
		ft_print_int(n / 10, print_len);
		ft_print_int(n % 10, print_len);
	}
}
