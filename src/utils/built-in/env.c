/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/04 19:32:01 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void    get_default_env(t_static *s)
{
    s->env= ft_lstnew(ft_strjoin("PWD=",s->pwd->content));
	add_list_and_sort(&(s->env),ft_lstnew(ft_strjoin("_=",s->last_cmd->content)));
	add_list_and_sort(&(s->env),ft_lstnew(ft_strjoin("OLDPWD=",s->oldpwd->content)));
    /* s->shlvl
     printf("str:%s\n", ft_itoa(1)); */
    add_list_and_sort(&(s->env),ft_lstnew(/* ft_strjoin("SHLVL=",s->shlvl->content) */"SHLVL=1"));
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

void    print_env_cpy(t_list *env)
{
    int i;
    t_list *temp;
    char *value;
    
    i = 0;
    temp = env;
    value = NULL;    
    while(temp)
    {
        value = ft_strchr(temp->content, '=');
        if (value && *(value + 1))
            printf("%s\n", temp->content);
        temp = temp->next;
    }
}

/* void    print_env_cpy(void)
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
} */

void    ft_env(char **input)
{
    t_static *s;
    
    s = ft_get_static();
    if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
        return ;
	}
    print_env_cpy(s->env);
}


/* void update_shlvl()
{
    t_static *s;
    
    s = ft_get_static();        
} */