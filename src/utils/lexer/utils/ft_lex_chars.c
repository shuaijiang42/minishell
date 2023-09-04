/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:34:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:35:04 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_chars(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		cmd->status = q_open;
	if (cmd->dollar_status == q_open)
	{
		cmd->spaces = 1;
		return (3);
	}
	return (2);
	c = 0;
}

