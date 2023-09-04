/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 19:34:29 by shujiang         ###   ########.fr       */
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

int main(int argc, char **argv, char **env)
{
	char *line;
	int		fd_mini_history;
	extern char **environ;

	//printf("%s", environ[0]);
	
	t_list	*history;
	t_static *s;
	(void)argc;
	(void)argv;
	//char pwd[4096];
	
	
	flag = 0;
	fd_mini_history = 0;
	//atexit(leaks);
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_DFL);
	//if (s->shlvl)
	//	shlvl++;
	ft_put_static(init_static_struct());
	s = ft_get_static();
	history = s->history;
	//s->pwd = getcwd(pwd, sizeof(pwd));
	/* printf("1spwd  %s\n", s->pwd);
	write(1 ,s->pwd, ft_strlen(s->pwd) + 120);
	printf("2spwd  %p\n", s->pwd); */
	
	ft_copy_env(env);
	//creat_exp_list(s);
	//ft_shlvl_sum();
	while (1)
	{
		/* s->pwd = getcwd(pwd, sizeof(pwd));
		ft_copy_env(env); */
		//creat_exp_list(s);
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			ft_free((void *)&line);
			exit(0);
		}
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
		{
			ft_procces_maker(line, env);
			s->oldpwd->content = ft_strdup(s->pwd->content);
			ft_put_proccess(0);
		}
		ft_free((void *)&line);
	}
	return (0);
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
