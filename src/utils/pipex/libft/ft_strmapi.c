/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 13:23:24 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/09 16:04:44 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const	*s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*result;

	i = 0;
	result = (char *)malloc((sizeof(char) * (ft_strlen(s) + 1)));
	if (!result || !f)
		return (NULL);
	ft_memcpy(result, s, ft_strlen(s) + 1);
	while (i < ft_strlen(s))
	{
		result[i] = (*f)(i, result[i]);
		i++;
	}
	return (result);
}
