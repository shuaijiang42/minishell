/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:16:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:17:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
//	1 si no es imprimible, 2 si es imprimible, 0 si es espacio y -1 es que ha acabado

int	ft_lex_quotes(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
	{
		cmd->status = q_open;
		if (c == '\'')
		{
			cmd->dollar_status = q_close;
			cmd->quotes = s_q;
			cmd->spaces = 3;
		}
		else
			cmd->quotes = d_q;
		return (1);
	}
	else
	{
		if (cmd->quotes == no_q)
		{
			if (c == '\'')
			{
				cmd->quotes = s_q;
				cmd->dollar_status = q_close;
				cmd->spaces = 3;
			}
			else
			{
				cmd->dollar_status = q_close;
				cmd->quotes = d_q;
				cmd->spaces = 3;
			}
			return (1);
		}
		else
		{
			if (cmd->quotes == s_q)
			{
				if (c == '\'')
				{
					cmd->dollar_status = q_close;
					cmd->quotes = no_q;
					cmd->spaces = 3;
					return (1);
				}
				else
					return (2);
			}
			else
			{
				if (c == '\"')
				{
					cmd->quotes = no_q;
					cmd->dollar_status = q_close;
					cmd->spaces = 3;
					return (1);
				}
				else
					return (2);
			}
		}
	
	}
}


