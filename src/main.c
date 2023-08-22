/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/22 13:27:05 by shujiang         ###   ########.fr       */
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
	(void)argc;
	(void)argv;
	
	fd_mini_history = 0;
	//atexit(leaks);
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	ft_put_static(init_struct(env));
	ft_sigaction();
	history = (ft_get_static())->history;
	while (1)
	{
		line = readline("minishell$ ");
		if (line != NULL)
		{
			add_history(line);
			ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		}
		if (ft_check_argument(line) == 1)
		{
			ft_procces_maker(line, env);
			ft_put_proccess(0);
			rl_redisplay();
		}
		else
			rl_on_new_line();
		ft_free((void *)&line);
	}
	return (0);
}
