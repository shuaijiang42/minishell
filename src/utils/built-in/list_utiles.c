/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:49:09 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_var_len(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	return (i);
}

t_list	*ft_locate_node(t_list *list, char *var_name)
{
	t_list	*temp;
	int		len;
	char	*str;

	if (!list)
	{
		printf("this is failing\n");
		return (NULL);
	}
	len = (int)ft_strlen(var_name);
	temp = list;
	str = NULL;
	while (temp)
	{
		str = temp->content;
		if (!ft_strncmp(str, var_name, len) && (str[len] == '='
				|| str[len] == '\0'))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_remove_node(t_list *list, int index)
{
	t_list	*before;
	int		i;
	t_list	*after;
	t_list	*temp;

	i = 0;
	temp = list;
	while (i < index - 1)
	{
		temp = temp->next;
		i++;
	}
	before = temp;
	temp = temp->next;
	after = temp->next;
	before->next = after;
	free(temp);
}

void custom_free(void *data) {
    free(data);
}

void	ft_node_substitute(t_list **old, t_list **new)
{
//	leaks();
	if (*old && *new)
	{
		printf("%s\n", (*old)->content);
		char *aux = ft_strdup((*new)->content);
		// free((*old)->content);
		(*old)->content = aux;
		//if (ft_strnstr(((*old)->content), "PWD=", ft_strlen((*old)->content)))
	
	}
	ft_lstclear(new, custom_free);
//	leaks();
}

void	ft_front_insert(t_list **front_node, t_list **new)
{
	t_list	*back_node;

	back_node = (*front_node)->next;
	(*front_node)->next = *new;
	(*new)->next = back_node;
}
