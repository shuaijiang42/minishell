/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_mode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 14:09:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 15:06:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	init_shell_mode(char **env, char **line, int *fd, t_list **history)
{
	g_flag = 0;
	*fd = 0;
	ft_get_old_history(env, fd);
	ft_put_history(*fd);
	*line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, quit_signal);
	*history = NULL;
	ft_lstadd_back(history, ft_lstnew((void *)ft_strdup("")));
}

/*
 		write(STDERR_FILENO, "exit\n", 5);
		ft_free((void *)line);
		ft_save_history();
		exit(ft_get_error());
 */
static void	shell_mode_util(int i, char **line, char **env)
{
	if (i == 1)
	{
		*line = readline("minishell$ ");
		g_flag = 0;
		return ;
	}
	else if (i == 2)
	{
		write(STDERR_FILENO, "exit\n", 5);
		ft_free((void *)line);
		ft_save_history();
		exit(ft_get_error());
	}
	else if (i == 3)
	{
		ft_procces_maker(*line, env);
		ft_put_proccess(0);
	}
}

static void	shell_init_proccess(t_static **s, t_list **history)
{	
	*s = ft_get_static();
	if (!*s)
		exit (-1);
	(*s)->history = *history;
	ft_put_error(0);
	g_flag = SHELL;
}

/*
	if (isatty(fileno(stdin)))
	{
		if (g_flag != 3)
			*line = readline("minishell$ ");
		else
		{
			*line = readline("minishell$ ");
			g_flag = 0;
		}
	}
	else
	{
		char *line2;
		line2 = get_next_line(fileno(stdin));
		*line = ft_strtrim(line2, "\n");
		free(line2);
	}
	(void)env;
*/
static void	init_flags_shell(char **env, char **line)
{		
	if (g_flag != 3)
		g_flag = SHELL;
	if (g_flag != 3)
		*line = readline("minishell$ ");
	else
		shell_mode_util(1, line, env);
}

int	shell_mode(char **env)
{
	char		*line;
	int			fd_mini_history;
	t_list		*history;
	t_static	*s;

	init_shell_mode(env, &line, &fd_mini_history, &history);
	ft_put_static(init_static_struct(env));
	shell_init_proccess(&s, &history);
	while (1)
	{
		init_flags_shell(env, &line);
		if (!line)
			shell_mode_util(2, &line, env);
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
			shell_mode_util(3, &line, env);
		else
			ft_free((void *)&line);
	}
	exit(ft_get_error());
}
