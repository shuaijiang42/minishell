/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/02 15:10:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	get_next_index_pipex(char *str)
{
	int	j;
	int	i;
	int	n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				return (i + 1);
		}
		i++;
	}
	return (i + 1);
}

size_t	count_pipes(char *str)
{
	int	j;
	int	i;
	int	n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				n += 1;
		}
		i++;
	}
	return (n);
}

int	ft_first_child(char *cmd, char **env, int pipe[2])
{
	int	pid;
	int	status;

	pid = fork_with_error_check();
	if(!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], 1);
		close(pipe[1]);
		exit(executer(cmd, env));
	}
	// i think i shuld close the write pipe here, but idk
	close(pipe[1]);
	waitpid(-1, &status, 0);
	return (pipe[0]);
}

int	ft_mid_child(char *cmd, char **env, int fd)
{
	int	pipe[2];
	int	pid;
	int	status;

	pipe_with_error_check(pipe);
	pid = fork_with_error_check();
	if(!pid)
	{
		dup2_with_error_check(fd, 0);
		close(fd);
		dup2_with_error_check(pipe[1], 1);
		close(pipe[1]);
		exit(executer(cmd, env));
	}
	close(fd);
	close(pipe[1]);
	waitpid(-1, &status, 0);
	return (pipe[0]);
}

int	ft_last_child(char *cmd, char **env, int fd)
{
	int	pid;
	int	status;

	pid = fork_with_error_check();
	if(!pid)
	{
		dup2_with_error_check(fd, 0);
		close(fd);
		exit(executer(cmd, env));
	}
	close(fd);
	waitpid(-1, &status, 0);
	return(WEXITSTATUS(status));
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = malloc(sizeof(char) * (n + 1));
	if (!str || !s1 || !n)
		return (ft_free((void **)&str));
	i = 0;
	j = 0;
	str[n] = '\0';
	while (i < n && s1[i])
		str[i++] = s1[j++];
	return (str);
}

char	*ft_get_cmd_pipex(char **cmd)
{
	size_t	nxt_cmd;
	char	*str;

	nxt_cmd = get_next_index_pipex(*cmd);
	if (nxt_cmd)
		nxt_cmd -= 1;
	str = ft_strndup(*cmd, nxt_cmd);
	*cmd += get_next_index_pipex(*cmd);
	return (str);
}

void	pipex(char *cmd, char **env)
{
	t_pipstr	pipex;
	int			pid;
	int			status;
	int			fd;

	status = 0;
	fd = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex.i = 1;
		pipex.cmd_cpy = cmd;
		pipex.n = count_pipes(cmd);
		pipex.status = 0;
		pipe_with_error_check(pipex.pipes.start_pipe);

		//first child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, env, pipex.pipes.start_pipe);
	
		//mid childs execution

		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, env, fd);
			pipex.i += 1;
		}

		//last child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, env, fd);
		close(0);
		close(pipex.pipes.start_pipe[1]);
		close(pipex.pipes.start_pipe[0]);
		close(fd);
		//printf("status before before pipes:%d, %d\n", WEXITSTATUS(pipex.status), pipex.status);
		exit (pipex.status);
	}
	waitpid(-1, &status, 0);
	//printf("status before pipes:%d, %d\n", WEXITSTATUS(status), status);
	exit (WEXITSTATUS(status));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char **input;
	int		pid;
	int		status;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
		{
			flag = 1;
			pid = fork_with_error_check();
			ft_put_proccess(1);
			if (!pid)
				pipex(cmd, env);
			waitpid(-1, &status, 0);
			ft_put_error(WEXITSTATUS(status));
		}
		else
			ft_put_error(executer(cmd, env));
	}
	else
		ft_free_split_2(&input);
}
