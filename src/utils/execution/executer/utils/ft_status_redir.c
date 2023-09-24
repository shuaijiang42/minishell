/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_status_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:19:46 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_put_status_redir(t_exc_lex *lex, char *input)
{
	input = lex->input;
	ft_init_cmd(&lex->cmd);
	lex->word = INACTIVE;
	if (lex->j == -1)
	{
		if (input[lex->i] == '<')
		{
			if (lex->status == non)
				lex->status = inp;
			else
				lex->status = hre;
		}
		else
		{
			if (lex->status == non)
				lex->status = trc;
			else
				lex->status = apd;
		}
	}
}
