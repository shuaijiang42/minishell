/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:58:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:38:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	init_alloc_parse(char ***res, t_alloc_parse *all)
{
	all->i = 0;
	all->x = 0;
	all->arg_len = 0;
	all->result = *res;
}

//	this funtion count how many numbers need per argument
void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
	t_alloc_parse	all;

	init_alloc_parse(result_ptr, &all);
	while (len)
	{
		all.arg_len = ft_lexer_len_argument(str);
		all.str2 = ft_calloc(sizeof(char), all.arg_len + 1);
		if (!all.str2)
			return ;
		ft_lexer_fill_str(str, &all.str2);
		all.str2[all.arg_len] = '\0';
		if (*all.str2)
			all.result[all.x++] = all.str2;
		else
		{
			if (*str != '$')
				all.result[all.x++] = all.str2;
			else
				ft_free((void **)&all.str2);
		}
		all.i = ft_lexer_get_next_argument(str);
		str += all.i;
		len--;
	}
}
