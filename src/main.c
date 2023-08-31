/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/31 14:40:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	leaks()
{
	system("leaks -q minishell");
}

void	*ft_free(void **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

void	ft_free_input(char **input)
{
	int i;
	i = 0;
	while (input[i])
	{
		free(input[i]);
		i++;
	}
	free (input);
}

void	*ft_print_error(char *str, int error)
{
	write(2, "\7minishell: ", 12);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	ft_put_error(error);
	errno = error;
	return (NULL);
}

int shell_mode(char **env)
{
	char *line;
	int		fd_mini_history;
	t_list	*history;
	
	fd_mini_history = 0;
	//atexit(leaks);
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	ft_put_static(init_struct(env));
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_DFL);
	history = (ft_get_static())->history;
	while (1)
	{
		flag = 0;
		line = readline("minishell$ ");
		if (!line)
		{
			//rl_redisplay();
			printf("exit\n");
			ft_free((void *)&line);
			exit(0);
		}
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
		{
			ft_procces_maker(line, env);
			ft_put_proccess(0);
			//rl_redisplay(); //not needed
		}
		else
		{
			//rl_on_new_line(); not needed
			ft_free((void *)&line);
		}
	}
	return (0);
}

int	exc_mode(char *file, char **env)
{
	char *str;
	char *gnl;

	ft_put_static(init_struct(env));
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
		ft_free((void **)&gnl);
		gnl = get_next_line_samu(fd);
	}
	return ();
}

int	main(int argc, char **argv, char **env)
{
	if (argc == 1)
		return (shell_mode(env));
	else 
		return (exc_mode(argv[1], env));
}

