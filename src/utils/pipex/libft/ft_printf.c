/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 10:30:36 by shujiang          #+#    #+#             */
/*   Updated: 2023/04/20 11:37:35 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_unsigned_int(unsigned int n, int base,
		int flag, int *print_len)
{
	static const char	str[23] = "0123456789abcdefABCDEF";

	if ((long long) n < (long long) base)
	{
		if (n > 9 && flag == 1)
			n += 6;
		*print_len += write (1, &str[n], 1);
	}	
	else
	{
		ft_print_unsigned_int(n / base, base, flag, print_len);
		ft_print_unsigned_int(n % base, base, flag, print_len);
	}
}

void	ft_print_unsigned_long(unsigned long n, int *print_len)
{
	static const char	str[17] = "0123456789abcdef";

	if (n < 16)
		*print_len += write (1, &str[n], 1);
	else
	{
		ft_print_unsigned_long(n / 16, print_len);
		ft_print_unsigned_long(n % 16, print_len);
	}
}

void	ft_print_ptr(void *ptr, int *print_len)
{
	unsigned long	ptr_value;

	ptr_value = (unsigned long)ptr;
	*print_len += ft_print_str("0x");
	ft_print_unsigned_long(ptr_value, print_len);
}

int	ft_format(va_list args, const char place_holder)
{
	int	print_len;

	print_len = 0;
	if (place_holder == 'c')
		print_len += ft_print_char(va_arg(args, int));
	if (place_holder == 's')
		print_len += ft_print_str(va_arg(args, char *));
	if (place_holder == 'p')
		ft_print_ptr(va_arg(args, void *), &print_len);
	if (place_holder == 'd' || place_holder == 'i')
		ft_print_int(va_arg(args, int), &print_len);
	if (place_holder == 'u')
		ft_print_unsigned_int(va_arg(args, unsigned int), 10, 0, &print_len);
	if (place_holder == 'x')
		ft_print_unsigned_int(va_arg(args, unsigned int), 16, 0, &print_len);
	if (place_holder == 'X')
		ft_print_unsigned_int(va_arg(args, unsigned int), 16, 1, &print_len);
	return (print_len);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		print_len;

	i = 0;
	print_len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			if (str[i] == '%')
				print_len += ft_print_char('%');
			else
				print_len += ft_format(args, str[i]);
		}
		else
			print_len += write(1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (print_len);
}
