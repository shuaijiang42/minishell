/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:56:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:21:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_lexer_len(t_lexer_len *len)
{
	len->len = 0;
	len->i = 0;
	len->j = 0;
	len->x = 0;
	ft_init_cmd(&len->cmd);
}

int	ft_lexer_len_argument(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (!len.j && str[len.i])
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j == 2)
			len.len += 1;
		if (len.j == 4)
			len.len += ft_dollar_len(str + len.i + 1, len.cmd);
		len.i += 1;
	}
	if (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0)
		{
			len.j = ft_check_char(&len.cmd, str[len.i]);
			if (len.j == 2)
				len.len += 1;
			if (len.j == 4)
				len.len += ft_dollar_len(str + len.i + 1, len.cmd);
			len.i += 1;
		}
	}
	return (len.len);
}

void	ft_lexer_len_loop(t_lexer_len *len, char *str)
{
	len->j = ft_check_char(&len->cmd, str[len->i]);
	if (len->j > 0 && len->x != 1)
	{
		len->len += 1;
		len->x = 1;
	}
	len->i += 1;
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (str[len.i] && !len.j && len.j != -1)
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j > 0)
		{
			len.len += 1;
			len.x = 1;
		}
		len.i += 1;
	}
	while (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0 && len.j != -1)
			ft_lexer_len_loop(&len, str);
		len.x = 0;
		ft_init_cmd(&len.cmd);
		while (str[len.i] && !len.j && len.j != -1)
			ft_lexer_len_loop(&len, str);
	}
	return (len.len);
}
