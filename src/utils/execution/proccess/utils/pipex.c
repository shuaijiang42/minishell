/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:57:06 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			status;
	int			fd;

	status = 0;
	fd = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex.i = 1;
		pipex.cmd_cpy = cmd;
		pipex.n = count_pipes(cmd);
		pipex.status = 0;
		pipe_with_error_check(pipex.pipes.start_pipe);

		//first child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
	
		//mid childs execution

		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			pipex.i += 1;
		}

		//last child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
		close(0);
		close(pipex.pipes.start_pipe[1]);
		close(pipex.pipes.start_pipe[0]);
		close(fd);
		//printf("status before before pipes:%d, %d\n", WEXITSTATUS(pipex.status), pipex.status);
		exit (pipex.status);
	}
	waitpid(-1, &status, 0);
	//printf("status before pipes:%d, %d\n", WEXITSTATUS(status), status);
	exit (WEXITSTATUS(status));
}
