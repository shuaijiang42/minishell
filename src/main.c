/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 14:28:17 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	leaks(void)
{
	system("leaks -q minishell");
}

void	ft_free_input(char **input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		free(input[i]);
		i++;
	}
	free (input);
}

/*
	char *str;
	char *gnl;
	t_static *s;

	ft_put_static(init_static_struct(env));
	s = ft_get_static();
	ft_put_error(0);
	errno = 0;
	int fd = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		ft_free((void **)&str);
		exit(errno);
	}
	size_t	i;

	i = 0;
	gnl = get_next_line_samu(fd);
	while (gnl)
	{
		i = 0;
		while (gnl[i])
		{
			if (gnl[i] == '\n')
			{
				gnl[i] = '\0';
				break ;
			}
			i++;
		}
		if (ft_check_argument(gnl) == 1)
			ft_procces_maker(gnl, env);
		gnl = get_next_line_samu(fd);
		ft_free((void **)&gnl);
	}
	return (0);
*/
int	exc_mode(void)
{
	write(STDERR_FILENO, "minishell: srry i am lazy\n", 26);
	exit (-1);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	if (argc == 1)
		return (shell_mode(env));
	else
		return (exc_mode());
}
