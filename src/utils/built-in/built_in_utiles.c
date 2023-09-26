/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:08:08 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	save_and_clear(void *content)
{
	char	*str;

	str = (char *)content;
	if (!str)
		return ;
	if (*str && (!((ft_get_history()) == STDIN_FILENO)))
	{
		ft_putstr_fd(str, ft_get_history());
		ft_putstr_fd("\n", ft_get_history());
	}
	ft_free((void **)&str);
	return ;
}

void	ft_save_history(void)
{
	t_list	*history;

	history = (ft_get_static())->history;
	ft_lstclear(&history, save_and_clear);
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
