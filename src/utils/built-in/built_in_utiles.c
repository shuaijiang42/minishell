/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 12:52:56 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	exit_with_no_argc(char **input)
{
	if (!ft_get_proccess())
		ft_save_history();
	ft_free_exit(input, ft_get_error());
}

void	not_numeric_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
	ft_put_error(255);
	if (!ft_get_proccess())
		ft_save_history();
	exit (255);
}
