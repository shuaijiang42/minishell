/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:58:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:28:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	this funtion count how many numbers need per argument
void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
////////////////////////////////////////////////////////////////////////////////
	char	**result;
	char	*str2;
	int		x;
	int		i;
	int		arg_len;

	i = 0;
	x = 0;
	arg_len = 0;
	result = *result_ptr;
////////////////////////////////////////////////////////////////////////////////
	while (len)
	{
		arg_len = ft_lexer_len_argument(str);
		str2 = ft_calloc(sizeof(char) , arg_len + 1);
		if (!str2)
			return ;
		ft_lexer_fill_str(str, &str2);
		str2[arg_len] = '\0';
		if (*str2)
			result[x++] = str2;
		else
		{
			if (*str != '$')
				result[x++] = str2;
			else
				ft_free((void **)&str2);
		}
		i = ft_lexer_get_next_argument(str);
		str += i;
		len--;
	}
////////////////////////////////////////////////////////////////////////////////
	return ;
	str = NULL;
	len = 0;
}
