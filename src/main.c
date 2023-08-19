/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 18:15:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/19 20:05:53 by samusanc         ###   ########.fr       */
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

void	executer(char *cmd, char **env)
{
	//this executer need to be improved, it does not count with the redirections
	char **input;

	input = ft_lexer(cmd);
	//this code belong to other place, i think the execute cmd, so maybe u can try to move it there?
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return ;
	}
	else
		ft_excuter(input, env);
}

void	pipex(char *cmd, char **env)
{
	int	n;

	n = count_pipes(cmd);
	//pipe = 
	//execute the first child,
	//execute the midle childs
	//execute the last child
	printf("this is the pipex way\n");
	executer(cmd, env);
}

void	ft_procces_maker(char *cmd, char **env)
{
	char **input;

	input = ft_lexer(cmd);
	if (input && *input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
			pipex(cmd, env);
		else
			executer(cmd, env);
	}
	else
		ft_free_split_2(&input);
	return ;
	env = NULL;
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
		ft_free((void *)&line);
		//leaks();
	}
	return (0);
}
