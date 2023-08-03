/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:07:01 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/03 15:29:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_error(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
}

void	here_doc(char *limiter, int fd)
{
	char	*s;
	char	*limit;

	limit = ft_strjoin(limiter, "\n");
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		s = get_next_line(STDIN_FILENO);
		if (!s)
			exit(1);
		if (ft_strcmp(s, limit) == 0)
		{
			free(s);
			free(limit);
			break ;
		}
		ft_putstr_fd(s, fd);
		free(s);
	}
}

t_var2	get_input_fd_for_cmd1(char **argv)
{
	int		end[2];
	t_var2	var;

	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		var.i = 3;
		pipe_with_error_check(end);
		here_doc(argv[2], end[WRITE]);
		close(end[WRITE]);
		var.fd = end[READ];
	}
	else
	{
		var.i = 2;
		var.fd = open(argv[1], O_RDONLY);
		if (var.fd == -1)
			perror(argv[1]);
	}
	return (var);
}

/*
int	main(int argc, char **argv, char **env)
{
	int		temp;
	t_var5	var5;
	t_var2	var2;

	if (argc < 5)
		print_error("Not enough arguments.");
	var2 = get_input_fd_for_cmd1(argv);
	var5.argc = argc;
	var5.argv = argv;
	var5.env = env;
	var5.i = var2.i;
	var5.fd = var2.fd;
	while (var5.i < var5.argc - 1)
	{	
		temp = execute_cmd(var5);
		var5.i++;
		var5.fd = temp;
	}
	return (0);
}
*/
