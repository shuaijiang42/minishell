/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:22:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:23:13 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_space(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		return (0);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			cmd->dollar_status = q_close;
			cmd->dollar = funtional;
			return (0);
		}
		else
		{
			if (cmd->quotes == d_q)
			{
				cmd->dollar_status = q_close;
				return (2);
			}
			else
				return (2);
		}
	}
	c = 0;
}
