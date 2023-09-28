/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:53:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:21:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_mid_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		ft_is_valid_in(fd, &input);
		close(fd);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(input.in);
	close(input.out);
	close(fd);
	close(pipe[1]);
	return (pipe[0]);
}
