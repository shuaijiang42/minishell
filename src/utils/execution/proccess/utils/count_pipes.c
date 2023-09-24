/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:45 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	count_pipes(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				n += 1;
		}
		i++;
	}
	return (n);
}
