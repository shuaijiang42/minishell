/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 10:36:25 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/09 16:04:03 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	if ((unsigned char)c == '\0')
	{
		while (*s != '\0')
			s++;
		return ((char *)s);
	}
	else
	{
		while (*s != (unsigned char)c && *s != '\0')
			s++;
		if (*s == '\0')
			return (0);
		else
			return ((char *)s);
	}
}
