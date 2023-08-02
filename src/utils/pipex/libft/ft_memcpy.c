/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 15:38:06 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/09 16:03:21 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*ptr;

	ptr = dest;
	if (dest == '\0' && src == '\0')
		return (0);
	while (n > 0)
	{
		*(char *)dest = *(char *)src;
		dest++;
		src++;
		n--;
	}
	return (ptr);
}
