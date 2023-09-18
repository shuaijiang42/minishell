/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:35:21 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:21:33 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_dollar(t_cmd *cmd, char c)
{
	if (cmd->quotes != s_q)
	{
		if (cmd->spaces)
		{
			cmd->dollar_status = q_open;
			cmd->spaces = 0;
			return (4);
		}
		else
		{
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			return (3);
		}
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->spaces = 0;
		return (2);
	}
	c = 0;
}
