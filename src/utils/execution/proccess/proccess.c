/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:12:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	proccess_pipex(char *cmd)
{
	int	pid;
	int	status;

	g_flag = PROCCESS;
	pid = fork_with_error_check();
	ft_put_proccess(1);
	if (!pid)
	{
		g_flag = 4;
		pipex(cmd);
	}
	waitpid(-1, &status, 0);
	ft_put_error(WEXITSTATUS(status));
}

static void	proccess_solo(char *cmd)
{
	int		pid;
	t_input	line;

	pid = ft_exc_make_redir(cmd, &line);
	if (pid)
	{
		ft_put_error(pid);
		return ;
	}
	ft_is_valid_in(STDIN_FILENO, &line);
	ft_put_error(executer(cmd, &line));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char	**input;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
			proccess_pipex(cmd);
		else
			proccess_solo(cmd);
	}
	else
		ft_free_split_2(&input);
	env = NULL;
}
