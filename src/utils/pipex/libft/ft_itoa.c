/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:52:51 by shujiang          #+#    #+#             */
/*   Updated: 2023/05/06 17:59:52 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_digit(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count++;
	while (n)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		size;
	long	nb;
	char	*ret;

	size = ft_count_digit(n);
	nb = n;
	ret = (char *)malloc(sizeof(char) * (size + 1));
	if (!ret)
		return (0);
	if (nb < 0)
	{
		ret[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		ret[0] = '0';
	ret[size--] = '\0';
	while (nb)
	{
		ret[size] = nb % 10 + '0';
		size--;
		nb = nb / 10;
	}
	return (ret);
}

/* int main()
{
	int n = -2147483648LL;
	int count = ft_count_digit(n);
	printf("%d\n", count);
	char *ret = ft_itoa(n);
	printf("%s", ret);
} */