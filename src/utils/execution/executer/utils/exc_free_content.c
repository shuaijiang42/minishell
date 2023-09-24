/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_free_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:13:43 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_free_content(void *cnt_ptr)
{
	t_argument	*content;

	content = (t_argument *)cnt_ptr;
	if (ft_strncmp(content->type, "cmd", 3))
		close(content->fd);
	ft_free((void **)&content->str);
	ft_free((void **)&content->type);
	return ;
}
