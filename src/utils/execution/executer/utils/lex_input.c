/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:34 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:01:42 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_exc_lex_input(char *input, int std[2])
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	ft_init_exc_lex(&lex);
	if (!input)
	{
		ft_print_error("syntax error near unexpected token `|'", 257);
		return (NULL);
	}
	else if (ft_check_dup_redir(input) == -1)
		return (NULL);
	result = ft_make_list(&lex);
	std[0] = lex.in;
	std[1] = lex.out;
	return (result);
}


