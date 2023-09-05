/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holaaaa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 15:32:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

char **ft_lexer(char *str)
{
	char			**result;
	int				len;
	int				i;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	// y si len da 0 que se hace???
	result = ft_calloc(sizeof(char *), (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
	str = NULL;
	i = 0;
}

