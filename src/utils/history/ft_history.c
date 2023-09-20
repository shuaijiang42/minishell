/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:21:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/20 16:24:50 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	save_and_clear(void *content)
{
	char *str;

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
