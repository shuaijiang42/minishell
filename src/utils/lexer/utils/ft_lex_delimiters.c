/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_delimiters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:17:59 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:18:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c)
{
	cmd->dollar_status = q_close;
	if (cmd->status == q_close)
		return (-1);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			return (-1);
		}
		else
			return (2);
	}
	c = 0;
}
