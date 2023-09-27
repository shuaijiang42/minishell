/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:47:40 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:48:03 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int var_len(char *str)
{
	int i;
	
	i = 0;
	if(!str)
		return (0);
	while(str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*var_existed(char *str)
{
	int i;
    t_list *temp;
	int len;
	char *var;
	char *exp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->exp;
	if (!str)
		return (NULL);
	len = var_len(str);
	var = ft_substr(str, 0, len);
    while(temp)
    {
		exp = temp->content;
        if(exp && ft_strncmp(exp + 11, var, len) == 0
			&& ((exp + 11)[len] == '\0' || (exp + 11)[len] == '=' ))
			return (exp);
        temp = temp->next;
    }
	return (NULL);
}

char *add_exp_syntax(char *str)
{
	char *new1;
	char *new2;
	char *new3;
	
	new1= ft_substr(str, 0, var_len(str) + 1);
	new2 = ft_strjoin("declare -x ", new1);
	free (new1);
	new1 = ft_strjoin(new2, "\"");
	free (new2);
	new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
	new3 = ft_strjoin(new2, "\"");
	free (new2);
	new2 = ft_strjoin(new1 , new3);
	return (new2);
}

void add_new_var_exp(char *str)
{
	t_list *new;
	char *new1;
	char *new2;
	t_static *s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	if (!str)
		return ;
	if (ft_strchr(str, '=') && ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free (new1);
	}
	else
	{
		if(!ft_strchr(str, '='))
			new2 = ft_strjoin("declare -x ", str);
		else
			new2 = add_exp_syntax(str);
	}
	new = ft_lstnew(new2);
     add_list_and_sort(&(s->exp), new);
}

void add_new_var_env(char *str)
{
	t_list *new;
	t_static *s;

	s = ft_get_static();
	new = ft_lstnew(str);
    add_list_and_sort(&(s->env), new);
}
