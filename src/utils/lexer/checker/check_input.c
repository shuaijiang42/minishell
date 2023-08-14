/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 11:18:15 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 14:11:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <lexer.h>

//			ft_check_argument
//		return values:
//		1 if is a valid argument 
//		0 if is a empty argument
//		-1 if is a invalid argument
int	ft_check_argument(char *str)
{
	int	len;

	len = ft_get_next_command(str);
	if (len == -4)
		return (0);
	while (len > 0)
	{
		str += len;
		len = ft_get_next_command(str);
	}
	if (len == -1)
		return (-1);
	return (1);
}

void	*ft_free_split_2(char ***split)
{
	int	i;

	i = 0;
	while (split[0][i])
	{
		if (split[0][i++])
			ft_free((void *)&split[0][i]);
	}
	ft_free((void *)&split[0]);
	*split = NULL;
	return (NULL);
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
