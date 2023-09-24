/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_error_make_list(t_list **result, t_exc_lex *lex, int error)
{
	ft_init_exc_lex(lex);
	ft_exc_clear_content(result);
	if (error)
		err_unexpected(0, 0, 0);
	return (NULL);
}

int	err_unexpected(int minor, int major, char first)
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
