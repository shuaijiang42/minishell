/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:27:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_normal_char(t_exc_lex *lex, int *space)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		err_unexpected(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	*space = 0;
	return (0);
}
