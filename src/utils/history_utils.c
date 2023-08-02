/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 17:03:22 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/01 17:33:45 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list *ft_new_node(char *command)
{
	t_list	*history;
	
	history = malloc(sizeof(t_list));
	if (!history)
		return (NULL);
	history->content = command;
	history->prev = NULL;
	history->next = NULL;
	history->flag = 0;
	return (history);
}

