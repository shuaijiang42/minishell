/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/08/03 15:32:46 by shujiang         ###   ########.fr       */
=======
/*   Updated: 2023/08/03 15:54:24 by shujiang         ###   ########.fr       */
>>>>>>> da85bfb633c30877e5c52e986e7cb5f7a9a038ad
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

char **ft_lexer(char **argv)
{
	char **result;
	char	*str;
	unsigned int	i;

	i = 0;
	str = argv[0];
	result = ft_split(str, ' ');
	return (result);
	str = NULL;
}

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
		else if (input && !input[0])
			input[0] = '\0';
		else
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
