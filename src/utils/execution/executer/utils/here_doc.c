/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:23:40 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:13:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_here_doc_owo(t_argument *content, int pipes[2])
{
	char	*str;

	g_flag = HERE;
	content->str = cut_input(content->str, NULL);
	str = readline("> ");
	if (!str)
		exit (0);
	while (1)
	{
		if (!ft_strncmp(content->str, str, ft_strlen2(content->str) + 1))
			exit(1);
		ft_putstr_fd(str, pipes[1]);
		write(pipes[1], "\n", 1);
		free(str);
		str = readline("> ");
		if (!str)
			exit (0);
	}
	free(str);
	exit(0);
}

int	ft_exc_here_doc(t_argument *content, t_exc_lex *lex)
{
	int		pipes[2];
	int		pid;
	int		status;

	if (pipe(pipes))
		exit(errno);
	content->type = ft_strdup("hre");
	pid = fork();
	if (!pid)
		ft_here_doc_owo(content, pipes);
	waitpid(pid, &status, 0);
	g_flag = PROCCESS;
	if (WEXITSTATUS(status) == 2)
	{
		g_flag = 3;
		close(pipes[1]);
		close(pipes[0]);
		errno = 1;
		return (-1);
	}
	close(pipes[1]);
	lex->in = pipes[0];
	return (pipes[0]);
}
