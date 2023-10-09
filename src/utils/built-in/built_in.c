/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:19:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	ft_excuter(char **input, char **env)
{
	t_bool	built_in;
	int		pid;
	int		status;

	pid = 0;
	status = 0;
	errno = 0;
	built_in = ft_built_in(input);
	if (built_in == false)
	{
		g_flag = PROCCESS;
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
