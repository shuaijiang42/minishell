/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_interrogation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:36:11 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:36:29 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_interrogation(t_cmd *cmd, char c)
{
	if (cmd->dollar_status == q_open)
	{
		cmd->dollar_status = q_close;
		if (cmd->spaces)
		{
			cmd->spaces = 3;
			return (2);
		}
		else
		{
			cmd->spaces = 3;
			return (3);
		}
	}
	else
		return (2);
	c = 0;
}
