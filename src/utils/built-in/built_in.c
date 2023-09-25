/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/25 17:09:38 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ft_pwd(void)
{
	char buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}

int check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
	}
	return (1);
}

void ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	save_and_clear(void *content)
{
	char *str;

	str = (char *)content;
	if (!str)
		return ;
	if (*str && (!((ft_get_history()) == STDIN_FILENO)))
	{
		ft_putstr_fd(str, ft_get_history());
		ft_putstr_fd("\n", ft_get_history());
	}
	ft_free((void **)&str);
	return ;
}

void	ft_save_history(void)
{
	t_list	*history;

	history = (ft_get_static())->history;
	ft_lstclear(&history, save_and_clear);
}

void exit_with_no_argc(char **input)
{
	if (!ft_get_proccess())
			ft_save_history();
		ft_free_exit(input, ft_get_error());
}

void not_numeric_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
	ft_put_error(255);
	if (!ft_get_proccess())
		ft_save_history();
	exit (255);
}

void ft_exit(char **input)
{
	int i;

	i = 0; 
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		exit_with_no_argc(input);
	if (input[1])
	{
		if(!check_digit(input[1])) 
			not_numeric_error(input[1]);
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit (ft_get_error());
}


int	ft_built_in(char **input)
{
	if (input && ft_strcmp(input[0], "echo") == 0)
		ft_echo (input);
	else if (ft_strcmp(input[0], "cd") == 0)
		ft_cd(input[1]);
	else if (ft_strcmp(input[0], "pwd") == 0)
		ft_pwd ();
	else if (ft_strcmp(input[0], "export") == 0)
		ft_export (input);
	else if (ft_strcmp(input[0], "unset") == 0)
		ft_unset (input);
	else if (ft_strcmp(input[0], "env") == 0)
		ft_env (input);
	else if (ft_strcmp(input[0], "exit") == 0)
		ft_exit (input);  
	else
		return (false);
	return (true);
}

/* void ft_update_lstcmd(char **env)
{
    int i;
    t_static *s;

	s = ft_get_static(); 
    i = 0;
    while (env[i])
    {
        if (!ft_strncmp(env[i], "_=", 2))
        {
            s->last_cmd->content = ft_strdup(env[i]);
            add_list_and_sort(&(s->env), ft_lstnew(s->last_cmd->content));
        }
        i++;
    }
} */

int	ft_excuter(char **input, char **env)
{
	t_bool	built_in;
	int	pid;
	int status;
	
	pid = 0;
	status = 0;
	errno = 0;
	built_in = ft_built_in(input);
	//ft_update_lstcmd(env);
	if (built_in == false)
	{
		flag = PROCCESS;
		pid = fork_with_error_check();
		if (pid == 0)
			execve_with_error_check(input, env);
		waitpid(-1, &status, 0);
		ft_free_split_2(&input);
		return (WEXITSTATUS(status));
	}
	else
	{
		ft_free_split_2(&input);
		return (errno);
	}
}
