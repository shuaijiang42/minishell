/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:27:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_lexer_fill_str(char *str, char **str2)
{
	t_lexer_len	l;

	init_lexer_len(&l);
	while (!l.j && str[l.i])
	{
		l.j = ft_check_char(&l.cmd, str[l.i]);
		if (l.j == 2)
			str2[0][l.x++] = str[l.i];
		if (l.j == 4)
			ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
		l.i += 1;
	}
	if (str[l.i] && l.j >= 0)
	{
		while (str[l.i] && l.j > 0)
		{
			l.j = ft_check_char(&l.cmd, str[l.i]);
			if (l.j == 2)
				str2[0][l.x++] = str[l.i];
			if (l.j == 4)
				ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
			l.i += 1;
		}
	}
}
// debugg string: printf("[%d] = '%c', j = %d\n", i, str[i], j);

int	ft_lexer_get_next_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j)
		j = ft_check_char(&cmd, str[i++]);
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
			j = ft_check_char(&cmd, str[i++]);
		return (i);
	}
	return (ft_strlen(str));
}
