/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:10:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/03 15:43:05 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	wait_all_children(void)
{
	int	id;
	int	status;

	status = 0;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(status);
}

void	child_execute_preparation(int i, int *end, int argc, char **argv)
{
	int	outfile_fd;

	outfile_fd = 0;
	if (i < argc - 2)
	{
		dup2_with_error_check(end[WRITE], STDOUT_FILENO);
		close(end[WRITE]);
	}
	if (i == argc - 2)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0)
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_APPEND);
		else
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_TRUNC);
		dup2_with_error_check(outfile_fd, STDOUT_FILENO);
	}
}

void	execve_with_error_check(char **argv, char **env)
{
	char	*path;

	path = cmd_path(*argv, env);
	if (execve(path, ft_split(*argv, ' '), env) == -1)
	{
		perror(*argv);
		exit(1);
	}
}

int	execute_cmd(t_var5 var)
{
	int	end[2];
	int	pid;

	if (var.i < var.argc - 2)
		pipe_with_error_check(end);
	pid = fork_with_error_check();
	if (pid == 0)
	{
		if (var.fd == -1)
			exit(1);
		dup2_with_error_check(var.fd, STDIN_FILENO);
		close(var.fd);
		child_execute_preparation(var.i, end, var.argc, var.argv);
		execve_with_error_check(&var.argv[var.i], var.env);
	}
	if (var.i == var.argc - 2)
	{
		close(end[READ]);
		wait_all_children();
	}
	if (var.fd)
		close(var.fd);
	close(end[WRITE]);
	var.fd = end[READ];
	return (var.fd);
}
