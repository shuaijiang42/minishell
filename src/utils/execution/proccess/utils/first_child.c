/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:52:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_first_child(char *cmd, int pipe[2])
{
	int	pid;
	int	status;
	t_input	input;

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
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	//waitpid(-1, &status, 0);
	return (pipe[0]);
	status = 0;
}


