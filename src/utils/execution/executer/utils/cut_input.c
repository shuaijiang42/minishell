/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:21:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*cut_input(char *str, int *i)
{
	char	**split;
	char	*result;

	result = NULL;
	split = ft_lexer(str);
	if (!split)
		return (NULL);
	if (*split)
		result = ft_strdup(*split);
	ft_free_split_2(&split);
	if (!result)
	{
		if (i)
			*i = -1;
		return (str);
	}
	return (result);
}
