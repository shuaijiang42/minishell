/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_dup_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:18:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:18:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_dup_redir(char *input)
{
	t_exc_lex	lex;
	int			space;
	int			word;

	space = 0;
	word = 0;
	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		lex.j = ft_check_char(&lex.cmd, input[lex.i]);
		if (!lex.j || lex.j == -1)
		{
			if (!lex.j)
			{
				if (lex.major || lex.minor || lex.redirs)
					space = 1;
			}
			ft_init_cmd(&lex.cmd);
		}
		else
		{
			if(ft_check_parse_normal_char(&lex, &space) == -1)
				return (-1);
		}
		if (lex.j == -1 && (input[lex.i] == '<' || input[lex.i] == '>'))
		{
			if(ft_check_parse_redirs(&lex, &space) == -1)
				return (-1);
		}
		lex.i += 1;
	}
	return (0);
}

