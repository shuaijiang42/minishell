/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/04 19:40:19 by shujiang         ###   ########.fr       */
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

//Free the parsed input list
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
	str = argv[0];
	
	result = ft_split(str, ' ');
	return (result);
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

/* #if 1

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
#endif */


//Shuai: This is main for testing ft_excuter
int main(int argc, char **argv, char **env)
{
	char **input;
	char *line;
	(void)argv;
	
	line = NULL;
	input = NULL;
	if (argc != 1)
		return (0);
	while (1)
	{
		line = readline("minishell$ ");
		if (line != NULL)
			add_history(line);
		input = ft_split(line, ' ');
		free (line);
		ft_excuter(input, env);
	}
	return (0);
}
