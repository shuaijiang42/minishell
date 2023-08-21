/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:15:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/21 19:10:38 by samusanc         ###   ########.fr       */
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

void	ft_get_old_history(char **env, int *fd)
{
	int		i;
	char	*str;
	char	*str2;
	char	*str3;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return ;
	str = *(env + i);
	str += 5;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while(str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
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
	history = NULL;
	ft_put_static(init_struct(env));
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
			rl_redisplay();
		}
		else
			rl_on_new_line();
		ft_free((void *)&line);
	}
	return (0);
}
