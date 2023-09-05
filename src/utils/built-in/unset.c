/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 19:48:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int len;
	t_list *temp;
	int index;
	char *ref;
	t_static *s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while(temp)
	{
		if(ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content + 11;
		index++;
	}	
	if (!temp)
		return (-1);
	return (index);
}

int	get_var_index_env(char *var_name)
{
	int len;
	t_list *temp;
	int index;
	char *ref;
	t_static *s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while(temp)
	{
		if(ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content;
		index++;
	}	
	if (!temp)
		return (-1);
	return (index);
}

/* void	unset_var(int index)
{
	t_list **ptr;
	t_list *node_to_remove;
	t_static *s;

	s = ft_get_static();
	node_to_remove = NULL;
	ptr = &(s->env);
	while(index > 0 && *ptr != NULL)
	{   
		ptr = &((*ptr)->next);
		index--;
	}
	if (*ptr != NULL)
    {
        node_to_remove = *ptr;
        *ptr = (*ptr)->next;
        free(node_to_remove);
    }
} */

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list *node_to_remove;
	t_static *s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void    ft_unset(char **input)
{
	int i;
	int index1;
	int index2;
	t_static *s;

	s = ft_get_static();
	i = 1;
	if(input[i] == NULL)
		return ;
	while (input[i])
	{		
		index1 = get_var_index_env(input[i]);
		index2 = get_var_index_exp(input[i]);
		printf("2: %d\n", index2);
		if (index1 >= 0)
			unset_var(input[i], index1, s->env);
		if (index2 >= 0)
			unset_var(input[i], index2, s->exp);
		i++;	
	}	
}
