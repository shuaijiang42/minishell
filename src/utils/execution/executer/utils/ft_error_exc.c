/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:15:32 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_error_exc_unexpected_token(int minor, int major, char first)
{	
	if (first == 'A')
	{
		if (minor)
			ft_print_error("syntax error near unexpected token `<'", 258);
		if (major)
			ft_print_error("syntax error near unexpected token `>'", 258);
	}
	else if ((minor > 2) || (major == 2 && !minor))
		ft_print_error("syntax error near unexpected token `<'", 258);
	else if ((major > 2) || (minor == 2 && !major))
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (minor && !major && first == '<')
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (major && !minor && first == '>')
		ft_print_error("syntax error near unexpected token `<'", 258);
	else
		ft_print_error("syntax error near unexpected token `newline'", 258);
	return (-1);
}
