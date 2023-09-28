/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_first_child(char *cmd, int pipe[2])
{
	int		pid;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	return (pipe[0]);
}
