/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/30 18:24:27 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void    get_default_env(t_static *s)
{
    s->env= ft_lstnew(s->pwd);
	add_list_and_sort(&(s->env),ft_lstnew(s->last_cmd));
	add_list_and_sort(&(s->env),ft_lstnew(s->oldpwd));
	add_list_and_sort(&(s->env),ft_lstnew("SHLVL=1"));
}
void ft_copy_env(char **env)
{
    int i;
    t_list *new;
    t_static *s;

    s = ft_get_static();
    i = 1;
    new = NULL;
    if (*env == NULL)
	{
		get_default_env(s);
		return ;
	}	
    s->env = ft_lstnew(env[0]);
    while (env[i])
    {
        add_list_and_sort(&(s->env), ft_lstnew(env[i]));
        i++;
    }
}

void    print_env_cpy(void)
{
    int i;
    t_list *temp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->env;
    while(temp)
    {
        if (temp->content && *(ft_strchr(temp->content, '=') + 1))
            printf("%s\n", temp->content);
        temp = temp->next;
    }
}

void    ft_env(char **input)
{
    if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
        return ;
	}
    print_env_cpy();
}

