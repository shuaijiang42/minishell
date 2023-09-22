/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:55:06 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_last_child(char *cmd, int fd)
{
	int	pipe[2];
	int	pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
	{
		if (flag == 3)
			exit(1);
		else
		{
			close (pipe[1]);
			return (pipe[0]);
		}
	}
	pid = fork_with_error_check();
	if(!pid)
	{
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		//dup2_with_error_check(fd, input.in);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	//waitpid(-1, &status, 0);
	return (0);
}
