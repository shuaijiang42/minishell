/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:22:58 by samusanc         ###   ########.fr       */
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
	ft_free_split_2(&(his.env));
	if (his.fd == -1)
		return ;
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
