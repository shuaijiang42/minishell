/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/04 10:44:34 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static	void	pipex_init_pipex(t_pipstr *pipex, int *fd, char *cmd)
{
	*fd = 0;
	pipex->i = 1;
	pipex->cmd_cpy = cmd;
	pipex->n = count_pipes(cmd);
	pipex->status = 0;
}

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			fd;
	int			status;

	status = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex_init_pipex(&pipex, &fd, cmd);
		pipe_with_error_check(pipex.pipes.start_pipe);
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
		ft_free((void **)&pipex.cmd);
		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			ft_free((void **)&pipex.cmd);
			pipex.i += 1;
		}
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
	}
	waitpid(-1, &status, 0);
	exit (WEXITSTATUS(status));
}
