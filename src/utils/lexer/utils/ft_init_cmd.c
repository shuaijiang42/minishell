/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:48:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:49:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->quotes = no_q;
	cmd->dollar_status = q_close;
	cmd->status = q_close;
	cmd->spaces = 1;
}
