/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/30 18:38:50 by shujiang         ###   ########.fr       */
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

int main(int argc, char **argv, char **env)
{
	char *line;
	int		fd_mini_history;
	t_list	*history;
	t_static *s;
	(void)argc;
	(void)argv;
	
	fd_mini_history = 0;
	//atexit(leaks);
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_DFL);
	ft_put_static(init_static_struct());
	s = ft_get_static();
	history = s->history;
	ft_copy_env(env);
    //creat_exp_list(s);
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
			char pwd[4096];
			s->oldpwd = ft_strjoin("OLDPWD=", ft_strdup(s->pwd));
			s->pwd = ft_strjoin("PWD=", getcwd(pwd, sizeof(pwd)));  		
			ft_put_proccess(0);
			//rl_redisplay(); //not needed
		}
		else
		{
			//rl_on_new_line(); not needed
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
