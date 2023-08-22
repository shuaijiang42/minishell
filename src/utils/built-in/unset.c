/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:34:55 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/14 14:25:52 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	is_var(char *str)
{
	int len;
	t_list *temp;
	int index;
	char *str_env;
	t_static *g;

	g = ft_get_static();
	index = 0;
	temp = g->env_cpy;
	len = ft_strlen(str);
	str_env = temp->content;
	while(temp)
	{
		if(ft_strncmp(str_env, str, len - 1) == 0 && str_env[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			str_env = temp->content;
		index++;
	}	
	if (!temp)
		return (-1);
	return (index);
}

void	unset_var(int index)
{
	t_list **ptr;
	t_list *node_to_remove;
	t_static *g;

	g = ft_get_static();
	node_to_remove = NULL;
	ptr = &(g->env_cpy);
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
}
void    ft_unset(char **input)
{
	int i;
	int index;
	
	i = 1;
	if(input[i] == NULL)
		return ;
	while (input[i])
	{
		index = is_var(input[i]);
		if (index >= 0)
			unset_var(index);
		i++;	
	}	
}
	