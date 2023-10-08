/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/08 16:28:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
int	check_put_new_node_first(t_list **list, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(list, &new);
		return (1);
	}
	else if (ft_strcmp((*list)->content, new->content) > 0)
	{
		ft_lstadd_front(list, new);
		return (1);
	}
	return (0);
}
*/

// len = ft_var_len(insert) + 1; check until '='
// add or substitute and sort

/*
int	ft_substitute_or_insert(t_list **list, t_list *temp, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		printf("hola\n");
		ft_node_substitute(&(temp->next), &new);
		return (1);
	}
	if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
	{
		printf("adeu\n");
		ft_front_insert(&temp, &new);
		return (1);
	}
	return (0);
}
*/

/*
void	add_list_and_sort(t_list **list, t_list *new)
{
	t_list	*temp;

	temp = *list;
	if (check_put_new_node_first(list, new) == 1)
		return ;
	while (temp)
	{
		if (ft_substitute_or_insert(list, temp, new))
			break ;
		temp = temp->next;
		if (!temp)
			ft_lstadd_back(list, new);
	}
}
*/

int	ft_count_node(t_list *list)
{
	int		i;
	t_list	*temp;

	i = 0;
	temp = list;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**list_to_matrix(t_list *list)
{
	char	**matrix;
	int		n;
	int		i;
	t_list	*temp;

	i = 0;
	n = ft_count_node(list);
	temp = list;
	matrix = (char **)malloc(sizeof(char *) * (n + 1));
	if (!matrix)
		return (NULL);
	while (i < n)
	{
		matrix[i] = ft_strdup((char *)temp->content);
		temp = temp->next;
		i++;
	}
	matrix[i] = NULL;
	i = 0;
	return (matrix);
}
