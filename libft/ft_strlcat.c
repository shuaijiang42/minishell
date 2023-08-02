/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 22:06:55 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/09 16:04:27 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d;
	size_t	i;

	d = ft_strlen(dst);
	if (d >= dstsize)
		return (dstsize + ft_strlen(src));
	i = 0;
	while (src[i] && (i < dstsize - d - 1))
	{
		dst[d + i] = src[i];
		i++;
	}
	dst[d + i] = '\0';
	return (d + ft_strlen(src));
}
