/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/29 18:03:15 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<minishell.h>

int ft_var_len(char *var)
{
    int i;

    i = 0;
    while (var[i])
    {
        if (var[i] == '=')
            return (i);
        i++;
    }
    return (0);
}

void    ft_node_substitute(t_list **old, t_list **new)
{
    free((*old)->content);
    (*old)->content = ft_strdup((*new)->content);
}

void    ft_front_insert(t_list **list, t_list* front_node, t_list *new)
{
    t_list *back_node;

    back_node = front_node->next;
    front_node->next = new;
    new->next = back_node;
}

//len = ft_var_len(insert) + 1; check until '='
void    add_list_and_sort(t_list **list, t_list *new)
{
    int  len;
    t_list *temp;
    
    len = ft_var_len(new->content) + 1;
    temp = NULL; 
    if (!*list)
        *list = new;
    else
    {
        temp = *list;
        while (temp)
        {
            if (ft_strcmp(temp->content, new->content) < 0)
            {
                if (ft_strncmp(temp->next->content, new->content, len) == 0)
                    ft_node_substitute(&(temp->next), &new);
                else if(ft_strcmp(temp->next->content, new->content) > 0)
                    ft_front_insert(list, temp, new);
                break ;
            }  
            temp = temp->next;
        }   
    }
}
