/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/02 16:19:27 by shujiang         ###   ########.fr       */
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
		if (rl_replace_line(input, 1))
			rl_redisplay();
		free (input);
		//rl_on_new_line();
	}
	return (0);
}
/* int	main()
{
	char s;

	s = 0;
	write (1, "minishell$ ", 11);
	while (1)
	{
		while (read(0, &s, 1) > 0 && s != '\n')
			write(1, &s, 1);
		write (1, "\nminishell$ ", 12);
	}
	return (-1);
} */
