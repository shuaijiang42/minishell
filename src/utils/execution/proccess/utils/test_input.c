/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:53:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_is_valid_in(int fd, t_input *input)
{
	char	**split;
	size_t	i;
	int		pipes[2];

	i = 0;
	split = ft_lexer(input->cmd);
	if (!split)
		return ;
	while (split[i])
		i++;
	ft_free_split_2(&split);
	if (i == 1 && input->here)
	{
		pipe_with_error_check(pipes);
		close(pipes[1]);
		dup2_with_error_check(input->in, pipes[0]);
		return ;
	}
	//exit(0);
	dup2_with_error_check(fd, input->in);
	return ;
	(void)pipes;
}
