/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_new_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:58:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:58:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex)
{
	t_list		*result;
	t_argument	*content;

	content = malloc(sizeof(t_argument));
	if (!content)
		return (NULL);
	if (argument)
		content->str = ft_strdup(argument);
	else
		content->str = ft_strdup("");
	content->type = NULL;
	if (type == non)
		content->type = ft_strdup("cmd");
	else
		content->fd = ft_exc_open_fd(content, type, lex);
	//{
	//	printf("status of stdin after be opend:%c, fd:%d\n", (int)read(content->fd, NULL, 0), content->fd);
//	}
	if (!content->str || !content->type || content->fd == -1)
	{
		return (ft_exc_free_content((void *)content));
	}
	result = ft_lstnew((void *)content);
	if (!result)
		return (ft_exc_free_content((void *)content));
	return (result);
}


