/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 19:49:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_save_history(void)
{
	t_save_history	his;

	his.old_history = NULL;
	his.history = (ft_get_static())->history;
	his.fd = ft_get_history();
	if (his.fd != -1 && his.fd != STDIN_FILENO)
		close(his.fd);
	his.env = list_to_matrix((ft_get_static())->env);
	his.fd = fd_old_history_4_list(his.env, &his);
	if (his.fd == -1)
	{
		return ;
	}
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
