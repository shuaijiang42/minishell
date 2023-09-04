/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 12:11:48 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/29 16:41:39 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	if ((!s1 && s2) || (s1 && !s2))
		return (n);
	if (!s1 && !s2)
		return (0);
	while ((unsigned char )s1[i] == (unsigned char )s2[i]
		&& s1[i] && s2[i] && i < n -1)
		i++;
	return ((unsigned char )s1[i] - (unsigned char )s2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((unsigned char )s1[i] == (unsigned char )s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char )s1[i] - (unsigned char )s2[i]);
}
