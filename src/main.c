/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/11 20:09:47 by samusanc         ###   ########.fr       */
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


/* void	ft_shlvl_sum(void)
{
	char *var = "SHLVL";
	t_static *s;
	char *env_var;
	size_t len;
	
	s = ft_get_static();
	len = ft_strlen(var);
	while (s->env)
	{
		env_var = s->env->content;
		if(env_var && var && !ft_strncmp(env_var, var, len) 
			&& env_var[ft_strlen(var)]== '=')
		{
			int number;
			char	*str;
			extern char **environ;

			number = ft_atoi(env_var + len + 1);
			number++;
			str = ft_itoa(number);
			s->shlvl->content = str;
			//printf("%s\n", ft_lstnew(ft_strjoin("SHLVL=", str))->content);
			add_list_and_sort(&(s->env), ft_lstnew(ft_strjoin("SHLVL=", str)));
			environ[0] = ft_strdup(env_var + len + 1);
			return ;
		}
		s->env = s->env->next;
	}
	return ;
} */



int shell_mode(char **env)
{
	char	*line;
	int		fd_mini_history;
	
	t_list	*history;
	t_static *s;
	flag = 0;
	fd_mini_history = 0;
	//atexit(leaks);
	
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, quit_signal);
	//if (s->shlvl)
	//	shlvl++;
	ft_put_static(init_static_struct(env));
	s = ft_get_static();
	history = s->history;
//	printf("%s\n", s->shlvl->content);
	ft_copy_env(env);

	creat_exp_list(s);
	add_list_and_sort(&(s->exp), ft_lstnew(ft_strjoin("declare -x ",s->shlvl->content)));
	//export_to_real_env(s);
	//ft_shlvl_sum();
	
	ft_put_error(0);
	//creat_exp_list(s);
	flag = SHELL;
	//ft_shlvl_sum();
	while (1)
	{
		if (flag != 3)
			flag = SHELL;
		//line = readline("minishell$ ");
		if (isatty(fileno(stdin)))
		{
			if (flag != 3)
				line = readline("minishell$ ");
			else
			{
				line = readline("");
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
			return(ft_get_error());
		}
		
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
		{
			ft_procces_maker(line, env);
			ft_put_proccess(0);
		}
		else
			ft_free((void *)&line);
	}
	return (ft_get_error());
}

int	exc_mode(char *file, char **env)
{
	char *str;
	char *gnl;

	errno = 0;
	ft_put_static(init_static_struct());
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

int	main(int argc, char **argv, char **env)
{
	if (argc == 1)
		return (shell_mode(env));
	else 
		return (exc_mode(argv[1], env));
}

//This is a main to test the fuction add_list_and_sort
/* int main()
{
    t_list *list;
    char pwd[4096];

    t_list *temp;

    getcwd(pwd, sizeof(pwd));
    list = malloc(sizeof(t_list));
    list = ft_lstnew(ft_strjoin("PWD=",pwd));
	add_list_and_sort(&list, ft_lstnew("Bpple="));
    add_list_and_sort(&list, ft_lstnew("_=./minishell"));
    add_list_and_sort(&list, ft_lstnew("SHLVL=1"));
	add_list_and_sort(&list, ft_lstnew("Aanana"));
	add_list_and_sort(&list, ft_lstnew("Bpple=9"));
	add_list_and_sort(&list, ft_lstnew("AAA=9"));
	add_list_and_sort(&list, ft_lstnew("Aanana = BANANA"));
    
    temp = list;
    while(temp)
    {
        printf("%s\n", temp->content);
        temp = temp->next;
    }
    return (0);
}  */

/* This main is for testing env
int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	
	t_static *s;
    ft_put_static(init_static_struct());
	s = ft_get_static();
	ft_copy_env(env);
	print_env_cpy();
    return (0);
} */
