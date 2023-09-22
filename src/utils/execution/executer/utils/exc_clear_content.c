/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_clear_content.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:57:53 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:13:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_exc_clear_content(t_list **result)
{
	ft_lstclear(result, \
	(void (*)(void *))&ft_exc_free_content);
	return (NULL);
}
