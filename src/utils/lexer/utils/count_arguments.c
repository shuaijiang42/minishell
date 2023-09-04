/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:05:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:57:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
