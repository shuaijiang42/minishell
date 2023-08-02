/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/02 17:49:30 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char *input;

	while ((input = readline("minishell$ ")) != NULL)
	{
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			exit(0);
		}
		execve_with_error_check(input, env);
		add_history(input);
		rl_replace_line(input, 1);
		rl_redisplay();
		free (input);
		//rl_on_new_line();
	}
	return (0);
}
