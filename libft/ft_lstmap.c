/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 17:23:09 by shujiang          #+#    #+#             */
/*   Updated: 2023/02/08 21:15:22 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*loop(t_list *lst, t_list *save, void *(*f)(void *),
			void (*del)(void *))
{
	t_list	*new;
	t_list	*ptr;

	new = save;
	while (lst -> next)
	{
		lst = lst -> next;
		ptr = f(lst -> content);
		if (!ptr)
		{
			ft_lstclear(&save, del);
			return (0);
		}
		new -> next = ft_lstnew(ptr);
		if (!new -> next)
		{
			del(ptr);
			ft_lstclear(&save, del);
			return (0);
		}	
		new = new -> next;
	}
	new = NULL;
	return (save);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*save;
	void	*ptr;

	if (!lst || !f || !del)
		return (0);
	ptr = f(lst -> content);
	if (!ptr)
		return (0);
	new = ft_lstnew(ptr);
	if (!new)
	{
		del(ptr);
		return (0);
	}
	save = new;
	return (loop(lst, save, f, del));
}
