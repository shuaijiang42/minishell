/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 13:58:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 15:32:44 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count && ((SSIZE_MAX / count) < size))
		return (0);
	ptr = malloc (count * size);
	if (!ptr)
		return (0);
	ft_bzero (ptr, size * count);
	return (ptr);
}

//first, make sure the passing parameter is legal
//size_t has a maximum size, it is defined in limits.h.
//SIZE_MAX should be bigger than SIZE_MAX > count *size. 
//but to avoid the danger of count*size > SIZE_MAX, 
//should use SIZE_MAX /count to compare with size.
//put count before the (SIZE_MAX / count) < size) 
//in the if condition to check first if count is 0