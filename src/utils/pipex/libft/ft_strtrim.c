/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 15:25:14 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/09 17:15:05 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_start(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	counter;

	i = 0;
	j = 0;
	counter = 0;
	while (set[j])
	{
		if (s1[i] == set[j])
		{
			counter++;
			i++;
			j = -1;
		}			
		j++;
	}
	return (counter);
}

static int	count_end(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	int		counter;

	i = ft_strlen(s1) - 1;
	j = 0;
	counter = 0;
	while (i > 0 && j < ft_strlen(set))
	{
		while (s1[i] == set[j])
		{
			counter++;
			i--;
			j = 0;
		}
		j++;
	}
	if (counter == (int)ft_strlen(s1))
		counter = 0;
	return (counter);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	int		i;
	size_t	start;
	size_t	end;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return ((char *)s1);
	start = count_start(s1, set);
	end = count_end(s1, set);
	ret = malloc((sizeof(char) * (ft_strlen(s1)) - start - end + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (start < (int)ft_strlen(s1) - end)
	{
		ret[i] = s1[start];
		i++;
		start++;
	}
	ret[i] = '\0';
	return (ret);
}
