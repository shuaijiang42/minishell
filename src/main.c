/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/03 12:39:51 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

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
		pid = fork_with_error_check();
		if (pid == 0)
			execve_with_error_check(&input, env);
		wait(&status);
		rl_replace_line(input, 1);
		rl_redisplay();
		free (input);
	}
	return (0);
}
