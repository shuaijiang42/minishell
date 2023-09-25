/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:09:27 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_lexer_fill_str(char *str, char **str2)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (!len.j && str[len.i])
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j == 2)
			str2[0][len.x++] = str[len.i];
		if (len.j == 4)
			ft_dollar_fill(str + len.i + 1, len.cmd, &len.x, str2[0] + len.x);
		len.i += 1;
	}
	if (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0)
		{
			len.j = ft_check_char(&len.cmd, str[len.i]);
			if (len.j == 2)
				str2[0][len.x++] = str[len.i];
			if (len.j == 4)
				ft_dollar_fill(str + len.i + 1, len.cmd, &len.x, str2[0] + len.x);
			len.i += 1;
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
