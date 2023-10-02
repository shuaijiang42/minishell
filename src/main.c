/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:36:53 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* void	leaks()
{
	system("leaks -q minishell");
} */

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

void custom_free2(void *data)
{
    free(data);
}


int shell_mode(char **env)
{
	char	*line;
	int		fd_mini_history;
	
	t_list	*history;
	t_static *s;
	flag = 0;
	fd_mini_history = 0;
	
	ft_get_old_history(env, &fd_mini_history);
	ft_put_history(fd_mini_history);
	line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, quit_signal);
	
	history = NULL;
	
	ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup("")));
	ft_put_static(init_static_struct(env));
	s = ft_get_static();
	s->history = history;

	ft_copy_env(env);

	creat_exp_list(s);
	
	ft_put_error(0);
	flag = SHELL;
	while (1)
	{
		//leaks();
		if (flag != 3)
			flag = SHELL;
		//line = readline("minishell$ ");
		if (isatty(fileno(stdin)))
		{
			if (flag != 3)
			{
				line = readline("minishell$ ");
			}
			else
			{
				line = readline("minishell$ ");
				flag = 0;
			}
		}
		else
		{
			char *line2;
			line2 = get_next_line(fileno(stdin));
			line = ft_strtrim(line2, "\n");
			free(line2);
		}
		if (!line)
		{
	//		write(STDERR_FILENO, "exit\n", 5);
			ft_free((void *)&line);
			//printf("this is the error:%d\n", ft_get_error());
			ft_save_history();
			//ft_lstclear(&(s->exp), custom_free2);
			exit(ft_get_error());
		}
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
		{
			//leaks();
			ft_procces_maker(line, env);
			
			ft_put_proccess(0);
		}
		else
			ft_free((void *)&line);
	}
	/* printf("old: %s\n", s->oldpwd->content);
    printf("last: %s\n", s->last_cmd->content); */
	exit(ft_get_error());
}

int	exc_mode(char *file, char **env)
{
	char *str;
	char *gnl;
	//=========================================================================================
	//t_list	*history;
	t_static *s;

	ft_put_static(init_static_struct(env));
	s = ft_get_static();
	//history = s->history;
	ft_copy_env(env);

	creat_exp_list(s);
	ft_put_error(0);

	errno = 0;
	//=========================================================================================
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
}

/* typedef struct 
{
	void	**mem;
	size_t	capacity;
	size_t	len;
} t_mem;


void	mem_realloc(t_mem *mem)
{
	mem->capacity *= 2;
	void	**new = malloc(sizeof(void *) );
}

void	mem_push(t_mem *mem, void *ptr)
{
	if (mem->capacity == mem->len)
	{
		// relloc
	}
	mem->mem[mem->len] = 	ptr;
	mem->len += 1;
} */

int	main(int argc, char **argv, char **env)
{
	/* t_mem 	mem;

	mem.capacity = 5;
	mem.len = 0;
	mem.mem = malloc(sizeof(void *) * mem.capacity); */

	
	if (argc == 1)
		return (shell_mode(env));
	else 
		return (exc_mode(argv[1], env));
	
	
}
