/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procces.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/21 15:19:19 by samusanc         ###   ########.fr       */
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

int	executer(char *cmd, char **env)
{
	//this executer need to be improved, it does not count with the redirections
	char **input;

	input = ft_lexer(cmd);
	//this code belong to other place, i think the execute cmd, so maybe u can try to move it there?
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}

void	ft_first_child(char *cmd, char **env, int pipe[2])
{
	int	pid;

	pid = fork_with_error_check();
	if(!pid)
	{
		dup2_with_error_check(pipe[0], 0);
		dup2_with_error_check(pipe[1], 1);
		exit(executer(cmd, env));
	}
	// i think i shuld close the write pipe here, but idk
	close(pipe[0]);
	return ;
}

void	ft_mid_child(char *cmd, char **env, t_pipstr *pipex)
{
	int	pipe[2];
	int	pid;

	if (pipex->i == 1)
	{
		pipe[0] = pipex->pipes.start_pipe[0];
		pipe[1] = pipex->pipes.start_pipe[1];
	}
	else if (pipex->i == pipex->n - 1)
	{
		pipe[0] = pipex->pipes.end_pipe[0];
		pipe[1] = pipex->pipes.end_pipe[1];
	}
	else
		pipe_with_error_check(pipe);
	pid = fork_with_error_check();
	if(!pid)
	{
		dup2_with_error_check(pipex->pipes.fd, 0);
		dup2_with_error_check(pipe[1], 1);
		exit(executer(cmd, env));
	}
	close(pipe[0]);
	pipex->pipes.fd = pipe[1];
	return ;
}

void	ft_last_child(char *cmd, char **env, int pipe[2])
{
	int	pid;

	pid = fork_with_error_check();
	if(!pid)
	{
		dup2_with_error_check(pipe[0], 0);
		close(pipe[1]);
		exit(executer(cmd, env));
	}
	close(pipe[0]);
	return ;
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

	int pid;
	int	status;

	status = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex.i = 1;
		pipex.cmd_cpy = cmd;
		pipex.n = count_pipes(cmd);
		pipex.status = 0;
		pipe_with_error_check(pipex.pipes.start_pipe);
		pipe_with_error_check(pipex.pipes.end_pipe);

		//first child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		ft_first_child(pipex.cmd, env, pipex.pipes.start_pipe);
		dup2_with_error_check(pipex.pipes.fd, pipex.pipes.start_pipe[1]);
	
		//mid childs execution

		if (pipex.i < pipex.n)
		{
			while (pipex.i < pipex.n)
			{
				pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
				ft_mid_child(pipex.cmd, env, &pipex);
				pipex.i += 1;
			}
		}
		else
		{
			dup2_with_error_check(pipex.pipes.start_pipe[1],\
			pipex.pipes.end_pipe[1]);
			dup2_with_error_check(pipex.pipes.start_pipe[0],\
			pipex.pipes.end_pipe[0]);
		}

		//last child execution

		dup2_with_error_check(pipex.pipes.end_pipe[0], pipex.pipes.fd);
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		ft_last_child(pipex.cmd, env, pipex.pipes.end_pipe);
		close(pipex.pipes.end_pipe[0]);
		close(pipex.pipes.end_pipe[1]);
		waitpid(-1, &pipex.status, 0);
		exit (WEXITSTATUS(pipex.status));
	}
	waitpid(-1, &status, 0);
	exit (WEXITSTATUS(status));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char **input;
	int		pid;

	int		status;
	input = ft_lexer(cmd);
	if (input && *input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
		{
			pid = fork_with_error_check();
			if (!pid)
				pipex(cmd, env);
			waitpid(-1, &status, 0);
			ft_put_error(WEXITSTATUS(status));
		}
		else
		{
			status = executer(cmd, env);
			ft_put_error(status);
		}
	}
	else
		ft_free_split_2(&input);
	//exit(0);
}


