/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 14:44:45 by shujiang          #+#    #+#             */
/*   Updated: 2023/04/27 13:33:31 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char*s2)
{
	char	*str;
	char	*ret;

	if (s1 == NULL)
		return ((char *)s2);
	if (s2 == NULL)
		return ((char *)s1);
	str = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	ret = str;
	if (!str)
		return (NULL);
	while (*s1)
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}
