/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:56:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:23:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lexer_len_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (!j && str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == 2)
			len += 1;
		if (j == 4)
			len += ft_dollar_len(str + i + 1, cmd);
		i++;
	}
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
		{
			j = ft_check_char(&cmd, str[i]);
			if (j == 2)
				len += 1;
			if (j == 4)
				len += ft_dollar_len(str + i + 1, cmd);
			i++;
		}
	}
	return (len);
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;
	int				x;

	len = 0;
	i = 0;
	j = 0;
	x = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j && j != -1)
	{
		j = ft_check_char(&cmd, str[i++]);
		if (j > 0)
		{
			len++;
			x = 1;
		}
	}
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0 && j != -1)
		{
			j = ft_check_char(&cmd, str[i++]);
			if (j > 0 && x != 1)
			{
				len++;
				x = 1;
			}
		}
		x = 0;
		ft_init_cmd(&cmd);
		while (str[i] && !j && j != -1)
		{
			j = ft_check_char(&cmd, str[i++]);
			if (j > 0 && x != 1)
			{
				len++;
				x = 1;
			}
		}
	}
	return (len);
}
