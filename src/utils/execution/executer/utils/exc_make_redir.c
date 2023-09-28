/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:15:31 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_make_redir(char *cmd, t_input *line)
{
	int		std[2];
	t_list	*input;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
	{
		if (errno != 9)
			return (errno);
		else
			return (1);
	}
	line->in = dup(std[0]);
	if (std[0] != STDIN_FILENO)
		line->here = 1;
	else
		line->here = 0;
	line->out = dup(std[1]);
	if (line->in == -1 || line->out == -1)
		return (errno);
	line->cmd = ft_exc_make_cmd(&input);
	return (0);
}
