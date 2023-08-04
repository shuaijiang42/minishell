/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/04 15:51:58 by shujiang         ###   ########.fr       */
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

/*
int	ft_lexer_check_status()
{

}
*/

void	*ft_print_error(char *str)
{
	write(2, "minishell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	return (NULL);
}

char **ft_lexer(char **argv)
{
	char			**result;
	char			*str;
	int				i;
	int				command;
	int				n_commands;
	t_command		status;

	i = 0;
	n_commands = 0;
	command = 0;
	str = argv[0];
	while (str[i] == ' ')
		i++;
	if (str[i])
	{
		n_commands = 1;
		if (str[i] == '|')
			return ((char **)ft_print_error("syntax error near unexpected token '|'"));
	//	ft_lexer_check_status(&status);
	}
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
	printf("%d", n_commands);
	result = ft_split(str, ' ');
	return (result);
	status.simple = q_close;
	str = NULL;
	i = 0;
}

#if 0
int	main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	ft_lexer(argv + 1);
	
	return (0);
	argc = 0;
}
#endif

#if 1
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
