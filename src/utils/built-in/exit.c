/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:05:15 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:07:19 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
	}
	return (1);
}

void	ft_exit(char **input)
{
	int	i;

	i = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		exit_with_no_argc(input);
	if (input[1])
	{
		if (!check_digit(input[1]))
			not_numeric_error(input[1]);
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit (ft_get_error());
}
