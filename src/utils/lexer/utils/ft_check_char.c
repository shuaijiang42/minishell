/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:37:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:52:04 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c);

int	ft_check_char(t_cmd *cmd, char c)
{
	if (c == '\'' || c == '\"')
		return (ft_lex_quotes(cmd, c));
	else if (c == '$')
		return (ft_lex_dollar(cmd, c));
	else if (c == '|' || c == '<' || c == '>')
		return (ft_lex_delimiters(cmd, c));
	else if (!c)
		return (-1);
	else if (c == ' ')
		return (ft_lex_space(cmd, c));
	else if (c == '?')
		return (ft_lex_interrogation(cmd, c));
	else
		return (ft_lex_chars(cmd, c));
}
