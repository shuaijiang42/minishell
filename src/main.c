/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:15:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/21 11:46:50 by samusanc         ###   ########.fr       */
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
	//write(STDOUT_FILENO, "the first child is in execution\n", 32);
	dup2_with_error_check(pipe[0], STDIN_FILENO);
	dup2_with_error_check(pipe[1], STDOUT_FILENO);
	executer(cmd, env);
	exit(0);
}

void	ft_mid_child(char *cmd, char **env, int pipe[2])
{
	dup2_with_error_check(pipe[0], STDIN_FILENO);
	dup2_with_error_check(pipe[1], STDOUT_FILENO);
	executer(cmd, env);
	exit (0);
}

void	ft_last_child(char *cmd, char **env, int pipe[2])
{
	dup2_with_error_check(pipe[0], STDIN_FILENO);
	close(pipe[1]);
	executer(cmd, env);
	exit (0);
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
		pipex.pid_i = 0;
		pipe_with_error_check(pipex.pipe);
		pipex.pid = malloc(sizeof(int) * (pipex.n + 1));
		if (!pipex.pid)
			return ;
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.pid[pipex.pid_i] = fork_with_error_check();
		if (!pipex.pid[pipex.pid_i])
			ft_first_child(pipex.cmd, env, pipex.pipe);
		pipex.pid_i += 1;
		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			pipex.pid[pipex.pid_i] = fork_with_error_check();
			if (!pipex.pid[pipex.pid_i])
				ft_mid_child(pipex.cmd, env, pipex.pipe);
			pipex.pid_i += 1;
			pipex.i += 1;
		}
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.pid[pipex.pid_i] = fork_with_error_check();
		if (!pipex.pid[pipex.pid_i])
			ft_last_child(pipex.cmd, env, pipex.pipe);
		close(pipex.pipe[0]);
		close(pipex.pipe[1]);
		//waitpid(pipex.pid_i, &pipex.status, 0);
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
			ft_procces_maker(line, env);
		else
			rl_on_new_line();
		ft_free((void *)&line);
		rl_redisplay();
	}
	return (0);
}
