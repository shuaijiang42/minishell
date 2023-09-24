/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pipex_error_make_redir(int pipe[2])
{
	if (flag == 3)
		exit(1);
	else
	{
		close (pipe[1]);
		return (pipe[0]);
	}
}

int	ft_last_child(char *cmd, int fd)
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
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	return (0);
}
