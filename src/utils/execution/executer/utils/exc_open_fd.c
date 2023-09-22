/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_open_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:30:22 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:30:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex)
{
	if (type == inp)
		return (ft_exc_change_input(content, lex));
	else if (type == hre)
		return (ft_exc_here_doc(content, lex));
	else if (type == trc)
		return (ft_exc_change_output_trc(content, lex));
	else if (type == apd)
		return (ft_exc_change_output_apd(content, lex));
	return (-1);
}
