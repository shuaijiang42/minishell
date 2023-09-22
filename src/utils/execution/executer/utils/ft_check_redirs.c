/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:18:11 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_redirs(t_exc_lex *lex, int *space)
{
	if (*space)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, 'A'));
	if (lex->input[lex->i] == '<' && !lex->major)
	{
		if (!lex->redirs)
			lex->first = '<';
		lex->minor += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '<' && lex->major)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	if (lex->input[lex->i] == '>' && !lex->minor)
	{
		if (!lex->redirs)
			lex->first = '>';
		lex->major += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '>' && lex->minor)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	return (0);
}
