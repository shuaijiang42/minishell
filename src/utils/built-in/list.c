/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/19 16:01:02 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int ft_var_len(char *var)
{
    int i;

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

t_list *ft_locate_node(t_list *list, char *var_name)
{
    t_list *temp;
    int len;
    char *str;


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
        if (!ft_strncmp(str, var_name, len) && (str[len] == '=' || str[len] == '\0'))
            return (temp);
        temp = temp ->next;
    }
    return (NULL);
}

void    ft_remove_node(t_list *list, int index)
{
    t_list *before;
    int i;
    t_list *after;
    t_list *temp;

    i = 0;
    temp = list;
    while (i < index - 1)
    {
       temp = temp ->next;
       i++; 
    }
    before = temp;
    temp = temp->next;
    after = temp->next;
    before->next = after;
    free(temp);
}

void    ft_node_substitute(t_list **old, t_list **new)
{
    if(*old && *new)
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
//add or substitute and sort
void    add_list_and_sort(t_list **list, t_list *new)
{
    int  len;
    t_list *temp;
    char *str;
    char *strnew;
    
    len = 0;
    strnew = NULL;
    temp = *list;
    str = (*list)->content;
    //printf("str %s\n",str);
    if (new)
	{
        strnew = new->content;
        //printf("strnew %s\n",strnew);
        len = ft_var_len(strnew);
        //printf("len %d\n",len);
    }    
    if (check_put_new_node_first(list, new) == 1)
		return ;
    while (temp)
    {
      /*printf("%d\n",len);
          printf("%c\n",str[len]);
        printf("%c\n",strnew[len]); */ 
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

int ft_count_node(t_list *list)
{
    int i;
    t_list *temp;
    
    i = 0;
    temp = list;
    while (temp)
    {
        i++;
        temp = temp->next;
    }
    return (i);
}

char **list_to_matrix(t_list *list)
{
	char **matrix;
    int n;
    int i;
    t_list *temp;

    i = 0;
    n = ft_count_node(list);
    temp = list;
    matrix = (char **)malloc(sizeof(char *) * (n + 1));
    if (!matrix)
        return (NULL); 
    while (i < n)
    {
        matrix[i] = ft_strdup((char *)temp->content);
        temp=temp->next;
        i++;
    }
    matrix[i] = NULL;
    i = 0;
    return (matrix);
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