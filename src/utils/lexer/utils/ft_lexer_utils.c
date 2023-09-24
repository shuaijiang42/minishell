/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:25:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


void	ft_lexer_fill_str(char *str, char **str2)
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
	while (!j && str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == 2)
			str2[0][x++] = str[i];
		if (j == 4)
			ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
		i++;
	}
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
		{
			j = ft_check_char(&cmd, str[i]);
			if (j == 2)
				str2[0][x++] = str[i];
			if (j == 4)
				ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
			i++;
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
