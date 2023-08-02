/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/02 17:25:09 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	
	char *input;

	while ((input = readline("minishell$ ")) != NULL)
	{
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			exit(0);
		}
		add_history(input);
		rl_replace_line(input, 1);
		rl_redisplay();
		free (input);
		//rl_on_new_line();
	}
	return (0);
}
