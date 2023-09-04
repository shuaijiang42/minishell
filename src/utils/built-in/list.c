/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/31 17:51:54 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int ft_var_len(char *var)
{
    int i;

    i = 0;
    while (var[i])
    {
        if (var[i] == '=')
            break ;
        i++;
    }
    return (i);
}

void    ft_node_substitute(t_list **old, t_list **new)
{
    (*old)->content = ft_strdup((*new)->content); 
}

void    ft_front_insert(t_list** front_node, t_list **new)
{
    t_list *back_node;
    
    back_node = (*front_node)->next;
    (*front_node)->next = *new;
    (*new)->next = back_node;
}

int   check_put_new_node_first(t_list **list, t_list *new)
{
	int  len;
  
    char *str;
    char *strnew;

    str = (*list)->content;
	strnew = new->content;
    len = ft_var_len(strnew);
    
    if (ft_strncmp(str, strnew, len) == 0 && str[len]== '=' && strnew[len] == '=')
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

//len = ft_var_len(insert) + 1; check until '='
void    add_list_and_sort(t_list **list, t_list *new)
{
    int  len;
    t_list *temp;
    
    char *str;
    char *strnew;
    
    temp = *list;
    str = (*list)->content;
	strnew = new->content;
    len = ft_var_len(strnew);
    
    if (check_put_new_node_first(list, new) == 1)
		return ;
    while (temp)
    {
        if (ft_strncmp(str, strnew, len) == 0 && str[len]== '=' && strnew[len] == '=')
        {
            ft_node_substitute(&(temp->next), &new);
            break ;
        }
        if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
        {
            ft_front_insert(&temp, &new);
            break ;
        }
        temp = temp->next;
        if(!temp)
            ft_lstadd_back(list, new);
    } 
}   

/* int main()
{
    t_list *list;
    char pwd[4096];

    t_list *temp;

    getcwd(pwd, sizeof(pwd));
    list = malloc(sizeof(t_list));
    list = ft_lstnew(ft_strjoin("PWD=",pwd));
	add_list_and_sort(&list, ft_lstnew("Bpple="));
    add_list_and_sort(&list, ft_lstnew("_=./minishell"));
    add_list_and_sort(&list, ft_lstnew("SHLVL=1"));
	add_list_and_sort(&list, ft_lstnew("Aanana"));
	add_list_and_sort(&list, ft_lstnew("Bpple=9"));
	
    
    temp = list;
    while(temp)
    {
        printf("%s\n", temp->content);
        temp = temp->next;
    }
    return (0);
} */