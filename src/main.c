/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/05 18:58:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void	*ft_free(void **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

int	count_arguments(char *str)
{
	int				i;
	int				command;
	int				n_commands;

	if (!str)
		return (0);
	i = 0;
	n_commands = 0;
	command = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		n_commands = 1;
	while (str[i])
	{
		while (str[i] != ' ' && str[i])
			i++;
		command = 0;
		while (str[i] == ' ' && str[i])
			i++;
		if (str[i] && str[i] != ' ' && !command)
			command += n_commands++;
		i++;
	}
	return (n_commands);
}

int	dollar_delimiter(char c)
{
	if (!c || c == '$' || c == '\'' || c == '\"' \
		|| c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

int	ft_lexer_check_status(t_command *cmd, char *str, int *i)
{
	char	c;

	c = str[*i];
	if (cmd->status == q_open)
	{
		if (cmd->simple_q == q_close && cmd->double_q == q_close \
		&& (str[*i] == '|' || str[*i] == '<' || str[*i] == '>'))
		{
			cmd->status = q_close;
			return (1);
		}
		else if (str[*i] == '\'' ||	str[*i] == '\"')
		{
			if (cmd->simple_q == q_open || cmd->double_q == q_open)
			{
				if (str[*i] == '\'' && cmd->simple_q == q_open)
				{
					if (!str[*i + 1] || str[*i + 1] == ' ')
						cmd->status = q_close;
					cmd->simple_q = q_close;
					cmd->dollar = funtional;
				}
				else if (str[*i] == '\"' && cmd->double_q == q_open)
				{
					if (!str[*i + 1] || str[*i + 1] == ' ')
						cmd->status = q_close;
					cmd->double_q = q_close;
				}
			}
			else if (str[*i] == '\'' && cmd->simple_q == q_close && cmd->simple_q == q_close)
			{
				cmd->simple_q = q_open;
				cmd->status = q_open;
			}
			else if (str[*i] == '\"' && cmd->simple_q == q_close && cmd->simple_q == q_close)
			{
				cmd->double_q = q_open;
				cmd->status = q_open;
			}
		}
		else if ((str[*i] == ' ' || !str[*i]) && cmd->simple_q == q_close && cmd->double_q == q_close)
			cmd->status = q_close;
		else if (str[*i] == '$' && cmd->simple_q == q_close)
		{
			if (str[*i + 1] == '$')
				*i += 1;
			while (!str[*i + 1] && dollar_delimiter(str[*i + 1]))
				*i += 1;
		}
		*i += 1;
	}
	return (0);
}

void	*ft_print_error(char *str)
{
	write(2, "minishell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	return (NULL);
}


//			ft_get_next_command
//		return values:
//		-3 if is the last or only command
//		-2 if is one pipe open
//		-1 if the quotes is no closed
//		(other value) index of the next command in the array

int ft_get_next_command(char *str)
{
	int				i;
	int				command;
	int				n_commands;
	int				end;

	t_command		status;
	status.dollar = funtional;
	status.simple_q = q_close;
	status.double_q = q_close;
	status.dollar = funtional;
	i = 0;
	n_commands = 0;
	command = 0;
	end = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		status.status = q_open;
	while (str[i] && !end)
	{
		while (status.status == q_open && str[i])
		{
			if(ft_lexer_check_status(&status, str, &i))
			{
				end = 1;
				break ;
			}
		}
		n_commands += 1;
		while (str[i] == ' ' && str[i] && !end)
			i++;
		if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
		{
			if(!str[i + 1])
				return (-2);
			else
				return (i + 1);
		}
		if (str[i] && !end)
		{
			if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
				break ;
			status.status = q_open;
		}
	}
	if (!str[i] && status.simple_q == q_close && status.double_q == q_close)
		status.status = q_close;
	if (status.status == q_open)
	{
		ft_print_error("syntax error unclosed quotes");
		return (-1);
	}
	return (-3);
}

int	count_arguments_lexer(char *str)
{
	int				i;
	int				command;
	int				n_commands;
	int				end;
	t_command		status;
	status.dollar = funtional;
	status.simple_q = q_close;
	status.double_q = q_close;
	status.dollar = funtional;

	i = 0;
	n_commands = 0;
	command = 0;
	end = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		status.status = q_open;
	while (str[i] && !end)
	{
		while (status.status == q_open && str[i])
		{
			if(ft_lexer_check_status(&status, str, &i))
			{
				end = 1;
				break ;
			}
		}
		n_commands += 1;
		while (str[i] == ' ' && str[i] && !end)
			i++;
		if (str[i] && !end)
		{
			if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
				break ;
			status.status = q_open;
		}
	}
	if (!str[i] && status.simple_q == q_close && status.double_q == q_close)
		status.status = q_close;
	if (status.status == q_open)
	{
		ft_print_error("syntax error unclosed quotes");
		return (-1);
	}
	return (n_commands);
}

char **ft_lexer(char **argv)
{
	char			**result;
	char			*str;
	int				len;

	str = argv[0];
	len = count_arguments_lexer(str);
	if (len == -1)
		return (NULL);
	result = ft_split(str, ' ');
	return (result);
	str = NULL;
}

#if 1
int	main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	printf("%d", ft_get_next_command(*(argv + 1)));
	//ft_lexer(argv + 1);
	
	return (0);
	argc = 0;
}
#endif

#if 0
int main(int argc, char **argv, char **env)
{
	char *input;
	int	pid;
	int	status = 0;
	
	if(argc > 1)
	{
		(void)argv;
		printf("Too many arguments\n");
		exit(0);
	}
	while (1)
	{
		input = readline("minishell$ ");
		if (input != NULL)
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free (input);
			exit (0);
		}
		/*
		if (ft_strncmp(input, "echo", 4) == 0)
		{
			printf("%s\n", input);
			//this is a built in, i will comment this because i need try the "" and ', 
			//so if u like you can implement the echo built in 
		
		}
		*/
		else if (!(input && !count_arguments(input)))
		{
			pid = fork_with_error_check();
			if (pid == 0)
				execve_with_error_check(&input, env);
			wait(&status);
			rl_replace_line(input, 1);
			rl_redisplay();
			free (input);
		}
	}
	return (0);
}
#endif
