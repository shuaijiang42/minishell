/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_lex_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:59:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_lex_word(t_list **result, t_exc_lex *lex)
{
	t_list	*tmp;
	char	*wrd;

	wrd = ft_exc_make_word(lex->input + lex->i);
	tmp = NULL;
	tmp = ft_exc_new_node(wrd, lex->status, lex);
	lex->status = non;
	ft_free((void **)&wrd);
	ft_lstadd_back(result, tmp);
	if (!tmp)
		return (-1);
	return (0);
}
