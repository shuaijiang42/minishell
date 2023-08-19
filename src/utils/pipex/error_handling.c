/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 12:21:27 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/19 17:12:20 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipe_with_error_check(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Pipe");
		exit(EXIT_FAILURE);
	}
}

void	dup2_with_error_check(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
}

int	open_with_error_check(const char *path, int oflag)
{
	int	fd;

	fd = open(path, oflag, 0666);
	if (fd == -1)
	{
		perror(path);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	fork_with_error_check(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	exit_cmd_not_found(char *path, char *temp, char *cmd)
{
	free(path);
	free(temp);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	errno = 127;
	exit(127);
}
